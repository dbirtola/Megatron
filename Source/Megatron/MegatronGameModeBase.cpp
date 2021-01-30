// Copyright Epic Games, Inc. All Rights Reserved.


#include "MegatronGameModeBase.h"
#include "Framework/MegatronPlayerController.h"
#include "Spawner/Spawner.h"
#include "FunctionLibraries/MegatronFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	// Spawn player slimes
	if (AMegatronPlayerController* MegatronPlayerController = UMegatronFunctionLibrary::GetMegatronPlayerController(this))
	{
		FTeam Team = MegatronPlayerController->GetTeam();
		PlayerSpawner->SetTeam(Team);
		PlayerSpawner->SpawnTeam();
	}

	FTeam EnemyTeam = GetNextEnemyTeam();
	EnemySpawner->SetTeam(EnemyTeam);
	EnemySpawner->SpawnTeam();


	// Spawn enemy slimes

}

void AMegatronGameModeBase::StartCombat()
{
	RoundState = ERoundState::PLAYER_TURN;
}

void AMegatronGameModeBase::FinishCombat()
{
	RoundState = ERoundState::NOT_STARTED;
}

void AMegatronGameModeBase::TickCombat()
{
	switch (RoundState)
	{
	case ERoundState::NOT_STARTED:
		break;
	case ERoundState::PLAYER_TURN:
		if (!SideHasTurnsPending())
		{
			FinishPlayerTurn();
		}
		break;
	case ERoundState::ENEMY_TURN:
		if (!SideHasTurnsPending())
		{
			FinishEnemyTurn();
		}
 		break;
	case ERoundState::LEARN_ABILITIES:
		if (false /*Check if enemy turn is over*/)
		{
			FinishLearnAbilitySegment();
		}

		break;
	case ERoundState::FORGET_ABILITIES:
		if (false /*Check if enemy turn is over*/)
		{
			FinishForgetAbilitySegment();
		}

	case ERoundState::FINISHED:
		StartNextRound();
		break;
	default:
		checkNoEntry();
		StartNextRound();
		break;
	}
}

void AMegatronGameModeBase::FinishRound()
{
	RoundState = ERoundState::FINISHED;
}

void AMegatronGameModeBase::StartNextRound()
{
	RoundState = ERoundState::PLAYER_TURN;
}

void AMegatronGameModeBase::StartPlayerTurn()
{
	RoundState = ERoundState::PLAYER_TURN;
	// All player slimes spawned will get a chance to take their turn
	SlimesWithTurnPending = GetSpawnedPlayerSlimes();
	ResetSlimesTurns(SlimesWithTurnPending);
	OnTurnStart(true);
}

void AMegatronGameModeBase::FinishPlayerTurn()
{
	OnTurnEnd(true);
	StartEnemyTurn();
}

void AMegatronGameModeBase::StartEnemyTurn()
{
	RoundState = ERoundState::ENEMY_TURN;
	// All enemy slimes spawned will get a chance to take their turn
	SlimesWithTurnPending = GetSpawnedEnemySlimes();
	ResetSlimesTurns(SlimesWithTurnPending);
	OnTurnStart(false);
}

void AMegatronGameModeBase::FinishEnemyTurn()
{
	OnTurnEnd(false);
	StartLearnAbilitySegment();

}

void AMegatronGameModeBase::StartLearnAbilitySegment()
{
	RoundState = ERoundState::LEARN_ABILITIES;
}

void AMegatronGameModeBase::FinishLearnAbilitySegment()
{

	StartForgetAbilitySegment();
}

void AMegatronGameModeBase::StartForgetAbilitySegment()
{
	RoundState = ERoundState::FORGET_ABILITIES;

	TArray<ASlime*> CandidateSlimes;
	CandidateSlimes.Append(GetSpawnedPlayerSlimes());
	CandidateSlimes.Append(GetSpawnedEnemySlimes());

	// Pick which slime is going to forget the abilities
	if (ASlime* ForgettingSlime = DetermineSlimeToForgetAbility(CandidateSlimes))
	{
		ForgettingSlime->ForgetRandomAbility();
		UE_LOG(LogTemp, Log, TEXT("Selected %s to forget an ability"), *ForgettingSlime->GetName());
	}
	// TODO: Blueprints should probably call this after the proper effects. Alternatively, set up timers for each of the "non-gameplay segments" and blueprints can control game pace that way
	FinishForgetAbilitySegment();
}

void AMegatronGameModeBase::FinishForgetAbilitySegment()
{
	RoundState = ERoundState::FINISHED;
}

bool AMegatronGameModeBase::SideHasTurnsPending()
{
	for (ASlime* Slime : SlimesWithTurnPending)
	{
		if (Slime->bHasTurnAvailable)
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

void AMegatronGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickCombat();
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

TArray<ASlime*> AMegatronGameModeBase::GetSpawnedPlayerSlimes()
{
	return PlayerSpawner->GetSpawnedSlimeActors();
}

TArray<ASlime*> AMegatronGameModeBase::GetSpawnedEnemySlimes()
{
	return EnemySpawner->GetSpawnedSlimeActors();
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

void AMegatronGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PrepareCombat();
}
