// Copyright Epic Games, Inc. All Rights Reserved.


#include "MegatronGameModeBase.h"
#include "Framework/MegatronPlayerController.h"
#include "Spawner/Spawner.h"
#include "FunctionLibraries/MegatronFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/Slime.h"
#include "Components/HealthComponent.h"
#include "Abilities/AbilityBase.h"

FString GetGameStateString(EGameState GameState)
{
	switch (GameState)
	{
	case EGameState::MAIN_MENU:
		return FString("Main Menu");
	case EGameState::COMBAT:
		return FString("Combat");
	case EGameState::INTERMISSION:
		return FString("Intermission");
	case EGameState::GAME_OVER:
		return FString("Game Over");
	default:
		return FString("None");
	}
}

FString GetRoundStateString(ERoundState RoundState)
{
	switch (RoundState)
	{
	case ERoundState::NOT_STARTED:
		return FString("Not Started");
	case ERoundState::PLAYER_TURN:
		return FString("Player Turn");
	case ERoundState::ENEMY_TURN:
		return FString("Enemy Turn");
	case ERoundState::LEARN_ABILITIES:
		return FString("Learn Abilities");
	case ERoundState::FORGET_ABILITIES:
		return FString("Forget Abilities");
	case ERoundState::FINISHED:
		return FString("Finished");
	default:
		return FString("None");
	}
}



void AMegatronGameModeBase::PrepareCombat()
{

	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawner::StaticClass(), Spawners);
	for (AActor* Spawner : Spawners)
	{
		if (Spawner->Tags.Contains("Player"))
		{
			PlayerSpawner = Cast<ASpawner>(Spawner);
		}
		else if (Spawner->Tags.Contains("Enemy"))
		{
			EnemySpawner = Cast<ASpawner>(Spawner);
		}
	}

	if (!ensureMsgf(PlayerSpawner, TEXT("No player spawner is set. Unable to prepare combat.")))
	{
		return;
	}

	if (!ensureMsgf(EnemySpawner, TEXT("No enemy spawner is set. Unable to prepare combat.")))
	{
		return;
	}

	// Cleanup any dead slimes
	if (PlayerSpawner)
	{
		PlayerSpawner->CleanupDeadSlimes();
	}
	if (EnemySpawner)
	{
		EnemySpawner->CleanupDeadSlimes();
	}
	

	// Spawn player slimes
	if (AMegatronPlayerController* MegatronPlayerController = UMegatronFunctionLibrary::GetMegatronPlayerController(this))
	{
		// Hack cause im tired. Player is overwriting their team.
		if (PlayerSpawner->GetSpawnedSlimeActors(false).Num() == 0)
		{
			FTeam Team = MegatronPlayerController->GetTeam();
			PlayerSpawner->SetTeam(Team);
			PlayerSpawner->SpawnTeam();
		}

		TArray<ASlime*> PlayerSlimes = GetSpawnedPlayerSlimes();
		for (ASlime* Slime : PlayerSlimes)
		{
			Slime->FactionID = UMegatronFunctionLibrary::GetPlayerFactionID();
		}
	}

	FTeam EnemyTeam = GetNextEnemyTeam();
	EnemySpawner->SetTeam(EnemyTeam);
	EnemySpawner->SpawnTeam();

	TArray<ASlime*> EnemySlimes = GetSpawnedEnemySlimes();
	for (ASlime* Slime : EnemySlimes)
	{
		Slime->FactionID = UMegatronFunctionLibrary::GetEnemyFactionID();
	}
}

void AMegatronGameModeBase::StartCombat()
{
	EnterGameState(EGameState::COMBAT);
	StartPlayerTurn();
	OnCombatStart();
}

void AMegatronGameModeBase::FinishCombat()
{
	EnterGameState(EGameState::INTERMISSION);
	EnterRoundState(ERoundState::NOT_STARTED);
	OnCombatEnd();
}

void AMegatronGameModeBase::GameOver()
{
	EnterGameState(EGameState::GAME_OVER);
	OnGameOver();
}

void AMegatronGameModeBase::TickCombat()
{
	switch (RoundState)
	{
	case ERoundState::NOT_STARTED:
		ensureMsgf(true, TEXT("Combat should never be ticked if a round is not started"));
		break;
	case ERoundState::PLAYER_TURN:
		OnPlayerTurnTick();
		// Only check for finish this way if the player spawner exists, so we don't skip this segment on debug maps.
		if (PlayerSpawner && !SideHasTurnsPending() || GetSpawnedEnemySlimes().Num() == 0)
		{
			FinishPlayerTurn();
		}
		break;
	case ERoundState::ENEMY_TURN:
		OnEnemyTurnTick();
		// Only check for finish this way if the enemy spawner exists, so we don't skip this segment on debug maps.
		if (EnemySpawner && !SideHasTurnsPending() || GetSpawnedPlayerSlimes().Num() == 0)
		{
			//FinishEnemyTurn();
		}
 		break;
	case ERoundState::LEARN_ABILITIES:
		// End of this segment is controlled by a timer. See StartLearnAbilitySegment
		break;
	case ERoundState::FORGET_ABILITIES:
		// End of this segment is controlled by a timer. See StartForgetAbilitySegment
		break;
	case ERoundState::FINISHED:
		StartNextRound();
		break;
	default:
		checkNoEntry();
		StartNextRound();
		break;
	}

	// If we don't have these, its probably a test map.
	if (PlayerSpawner && EnemySpawner)
	{
		// Check if the player or enemy has died
		if (!PlayerHasSlimesAlive())
		{
			GameOver();
		}

	}
}

void AMegatronGameModeBase::FinishRound()
{
	EnterRoundState(ERoundState::FINISHED);
}

void AMegatronGameModeBase::StartNextRound()
{
	StartPlayerTurn();
}

void AMegatronGameModeBase::StartPlayerTurn()
{
	EnterRoundState(ERoundState::PLAYER_TURN);
	if (PlayerSpawner)
	{
		// All player slimes spawned will get a chance to take their turn
		SlimesWithTurnPending = GetSpawnedPlayerSlimes();
		ResetSlimesTurns(SlimesWithTurnPending);
	}
	OnTurnStart(true);
}

void AMegatronGameModeBase::FinishPlayerTurn()
{
	OnTurnEnd(true);

	if (!EnemyHasSlimesAlive())
	{
		FinishCombat();
	}
	else
	{
		StartEnemyTurn();
	}
}

void AMegatronGameModeBase::StartEnemyTurn()
{
	EnterRoundState(ERoundState::ENEMY_TURN);
	if (EnemySpawner)
	{
		// All enemy slimes spawned will get a chance to take their turn
		SlimesWithTurnPending = GetSpawnedEnemySlimes();
		ResetSlimesTurns(SlimesWithTurnPending);
		SimulateEnemyTurns();
	}
	OnTurnStart(false);
}

void AMegatronGameModeBase::FinishEnemyTurn()
{
	OnTurnEnd(false);
	StartLearnAbilitySegment();

}

void AMegatronGameModeBase::StartLearnAbilitySegment()
{
	EnterRoundState(ERoundState::LEARN_ABILITIES);

	GetWorld()->GetTimerManager().SetTimer(LearnAbilityTimerHandle, this, &AMegatronGameModeBase::FinishLearnAbilitySegment, LearnAbilitySegmentSeconds, false);
}

void AMegatronGameModeBase::FinishLearnAbilitySegment()
{
	GetWorld()->GetTimerManager().ClearTimer(LearnAbilityTimerHandle);
	StartForgetAbilitySegment();
}

void AMegatronGameModeBase::StartForgetAbilitySegment()
{
	EnterRoundState(ERoundState::FORGET_ABILITIES);

	TArray<ASlime*> CandidateSlimes;
	CandidateSlimes.Append(GetSpawnedPlayerSlimes());
	CandidateSlimes.Append(GetSpawnedEnemySlimes());

	// Pick which slime is going to forget the abilities
	if (ASlime* ForgettingSlime = DetermineSlimeToForgetAbility(CandidateSlimes))
	{
		ForgettingSlime->ForgetRandomAbility();
		UE_LOG(LogTemp, Log, TEXT("Selected %s to forget an ability"), *ForgettingSlime->GetName());
	}

	GetWorld()->GetTimerManager().SetTimer(ForgetAbilityTimerHandle, this, &AMegatronGameModeBase::FinishForgetAbilitySegment, ForgetAbilitySegmentSeconds, false);
}

void AMegatronGameModeBase::FinishForgetAbilitySegment()
{
	GetWorld()->GetTimerManager().ClearTimer(ForgetAbilityTimerHandle);
	EnterRoundState(ERoundState::FINISHED);
}

bool AMegatronGameModeBase::SideHasTurnsPending()
{
	for (ASlime* Slime : SlimesWithTurnPending)
	{
		if (Slime->HealthComponent->CurrentHealth > 0 && Slime->bHasTurnAvailable)
		{
			return true;
		}
	}
	return false;
}

void AMegatronGameModeBase::ResetSlimesTurns(TArray<ASlime*> Slimes)
{
	for (ASlime* Slime : SlimesWithTurnPending)
	{
		Slime->bHasTurnAvailable = true;
	}
}

bool AMegatronGameModeBase::PlayerHasSlimesAlive()
{
	TArray<ASlime*> PlayerSlimes = GetSpawnedPlayerSlimes();
	for (ASlime* Slime : PlayerSlimes)
	{
		if (Slime->HealthComponent->CurrentHealth > 0)
		{
			return true;
		}
	}
	return false;
}

bool AMegatronGameModeBase::EnemyHasSlimesAlive()
{
	TArray<ASlime*> EnemySlimes = GetSpawnedEnemySlimes();
	for (ASlime* Slime : EnemySlimes)
	{
		if (Slime->HealthComponent->CurrentHealth > 0)
		{
			return true;
		}
	}
	return false;
}

void AMegatronGameModeBase::EnterGameState(EGameState NewGameState)
{
	UE_LOG(LogTemp, Log, TEXT("Changing Game State from %s to %s"), *GetGameStateString(MegatronGameState), *GetGameStateString(NewGameState));
	MegatronGameState = NewGameState;
}

void AMegatronGameModeBase::EnterRoundState(ERoundState NewRoundState)
{
	UE_LOG(LogTemp, Log, TEXT("Changing Round State from %s to %s"), *GetRoundStateString(RoundState), *GetRoundStateString(NewRoundState));
	RoundState = NewRoundState;
}

void AMegatronGameModeBase::SimulateEnemyTurns()
{
	SimulateNextEnemyTurn();
}

PRAGMA_DISABLE_OPTIMIZATION

void AMegatronGameModeBase::SimulateNextEnemyTurn()
{
	// Select a random slime that still has moves
	TArray<ASlime*> SlimesWithMoves;
	for (ASlime* Slime : GetSpawnedEnemySlimes())
	{
		if (Slime->bHasTurnAvailable && Slime->HealthComponent->CurrentHealth > 0)
		{
			SlimesWithMoves.Add(Slime);
		}
	}

	if (SlimesWithMoves.Num() > 0)
	{

		int32 index = FMath::RandRange(0, SlimesWithMoves.Num() - 1);
		ASlime* SelectedSlime = SlimesWithMoves[index];

		// select random ability
		AAbility* Ability = nullptr;
		TArray<AAbility*> Abilities = SelectedSlime->GetAbilities();
		index = FMath::RandRange(0, Abilities.Num() - 1);
		Ability = Abilities[index];

		// Select a random target. TODO: Make sure slimes filter for reasonable targets, and don't cast buffs on enemies
		ASlime* Target = nullptr;
		TArray<ASlime*> TargetSlimes = GetSpawnedPlayerSlimes();
		index = FMath::RandRange(0, TargetSlimes.Num() - 1);
		Target = TargetSlimes[index];

		Abilities[index]->TryExecuteAbility(Target);

		// Wait a little and then check if we have to simulate any other enemy turns
		GetWorld()->GetTimerManager().SetTimer(SimulateEnemyTurnTimerHandle, this, &AMegatronGameModeBase::OnSimulateNextEnemyTurnFinished, 1.0f, false);
	}
	else
	{
		FinishEnemyTurn();
	}

}

PRAGMA_ENABLE_OPTIMIZATION

void AMegatronGameModeBase::OnSimulateNextEnemyTurnFinished()
{
	if (SideHasTurnsPending())
	{
		SimulateNextEnemyTurn();
	}
	else
	{
		FinishEnemyTurn();
	}
}

void AMegatronGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (MegatronGameState)
	{
	case EGameState::MAIN_MENU:
		break;
	case EGameState::INTERMISSION:
		// TODO: Rewards and junk. For now, we go straight into the next combat!
		PrepareCombat();
		StartCombat();
	case EGameState::COMBAT:
		TickCombat();
		break;
	case EGameState::GAME_OVER:
		// TODO: Display restart screen. Or display give us lots of money to keep going screen. idk i'm not a scientist.
		break;
	default:
		break;
	}
}



ASlime* AMegatronGameModeBase::DetermineSlimeToForgetAbility(TArray<ASlime*> CandidateSlimes)
{
	ASlime* SelectedSlime = nullptr;

	return nullptr;
}

FTeam AMegatronGameModeBase::GetNextEnemyTeam()
{
	// Generate a random team for now. TODO: Add a way to influence their load out to make them stronger as time goes on. And perhaps follow slime "archetypes"
	FTeam EnemyTeam;
	EnemyTeam.OwningController = nullptr; // Awful
	for (int i = 0; i < EnemyTeamSize; i++)
	{
		FSlimeInfo NewSlime;
		NewSlime.SlimeClass = DefaultEnemySlimeClass;
		EnemyTeam.SlimeInfos.Add(NewSlime);
	}

	return EnemyTeam;
}

void AMegatronGameModeBase::StartGame()
{
	MegatronGameState = EGameState::INTERMISSION;
}

TArray<ASlime*> AMegatronGameModeBase::GetSpawnedPlayerSlimes()
{
	return PlayerSpawner ? PlayerSpawner->GetSpawnedSlimeActors(true) : TArray<ASlime*>();
}

TArray<ASlime*> AMegatronGameModeBase::GetSpawnedEnemySlimes()
{
	return EnemySpawner ? EnemySpawner->GetSpawnedSlimeActors(true) : TArray<ASlime*>();
}

void AMegatronGameModeBase::DEBUG_ForceNextRoundState()
{
	switch (RoundState)
	{
	case ERoundState::NOT_STARTED:
		StartPlayerTurn();
		break;
	case ERoundState::PLAYER_TURN:
		FinishPlayerTurn();
		break;
	case ERoundState::ENEMY_TURN:
		FinishEnemyTurn();
		break;
	case ERoundState::LEARN_ABILITIES:
		FinishLearnAbilitySegment();
		break;
	case ERoundState::FORGET_ABILITIES:
		FinishForgetAbilitySegment();
		break;
	case ERoundState::FINISHED:
		StartNextRound();
		break;
	default:
		break;
	}
}

void AMegatronGameModeBase::DEBUG_ForceNextGameState()
{
	switch (MegatronGameState)
	{
	case EGameState::MAIN_MENU:
		EnterGameState(EGameState::INTERMISSION);
		break;
	case EGameState::COMBAT:
		EnterGameState(EGameState::INTERMISSION);
		break;
	case EGameState::INTERMISSION:
		EnterGameState(EGameState::COMBAT);
		break;
	case EGameState::GAME_OVER:
		EnterGameState(EGameState::MAIN_MENU);
		break;
	default:
		break;

	}
}

void AMegatronGameModeBase::DEBUG_ForceGameOver()
{
	GameOver();
}

void AMegatronGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
