// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Spawner/Spawner.h"
#include "Pawns/Slime.h"
#include "MegatronGameModeBase.generated.h"

class ASpawner;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	MAIN_MENU,
	COMBAT,
	INTERMISSION,
	GAME_OVER
};

UENUM(BlueprintType)
enum class ERoundState : uint8
{
	NOT_STARTED,
	PLAYER_TURN,
	ENEMY_TURN,
	LEARN_ABILITIES,
	FORGET_ABILITIES,
	FINISHED
};



/**
 * 
 */
UCLASS()
class MEGATRON_API AMegatronGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Begin AGameModeBase implementation
	void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
	// End AGameModeBase implementation

private:
	// Generates new enemies for the player to fight.
	void PrepareCombat();

	// Starts combat with enemies present on the field
	void StartCombat();
	void FinishCombat();

	void GameOver();

	// Polls current combat state to determine whether or not we should move to the next state. Might change this to be more event driven
	void TickCombat();

	void FinishRound();
	void StartNextRound();

	// Lots of these, but makes it easy to know exactly where to add new code.
	void StartPlayerTurn();
	void FinishPlayerTurn();
	void StartEnemyTurn();
	void FinishEnemyTurn();
	void StartLearnAbilitySegment();
	void FinishLearnAbilitySegment();
	void StartForgetAbilitySegment();
	void FinishForgetAbilitySegment();

	bool SideHasTurnsPending();
	void ResetSlimesTurns(TArray<ASlime*> Slimes);
	bool PlayerHasSlimesAlive();
	bool EnemyHasSlimesAlive();

	void ChangeGameState(EGameState NewGameState);
	void ChangeRoundState(ERoundState NewRoundState);


public:
	FTeam GetNextEnemyTeam();

	// Called when the player presses Play. Starts our first intermission, which will go into combat.
	UFUNCTION(BlueprintCallable)
	void StartGame();

	// Blueprint driven logic to determine which slime forgets things
	UFUNCTION(BlueprintCallable)
	virtual ASlime* DetermineSlimeToForgetAbility(TArray<ASlime*> CandidateSlimes);

	UFUNCTION(BlueprintPure)
	TArray<ASlime*> GetSpawnedPlayerSlimes();
	UFUNCTION(BlueprintPure)
	TArray<ASlime*> GetSpawnedEnemySlimes();


	// Functions for blueprints to respond to round state changes. Mostly for updating visuals and UI. Easier for BP to read than having blueprints switch on an enum.
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnStart(bool bIsPlayerTurn);
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnEnd(bool bWasPlayerTurn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCombatStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCombatEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	// Debug only! Use this to force the next round state. Aka player turn->enemy turn->learn abilities->forget abilities
	UFUNCTION(BlueprintCallable)
	void DEBUG_ForceNextRoundState();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EnemyTeamSize = 3;

	// Array of all slimes that still need to act before this turn is over.
	TArray<ASlime*> SlimesWithTurnPending;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ASlime> DefaultEnemySlimeClass;

	UPROPERTY(BlueprintReadOnly)
	EGameState MegatronGameState;

	UPROPERTY(BlueprintReadOnly)
	ERoundState RoundState;

	// Spawner for the player's team. Found at the start of the game by searching for a spawner with the Player tag
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ASpawner* PlayerSpawner;

	// Spawner for the enemy team. Found at the start of the game by searching for a spawner with the Enemy tag
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ASpawner* EnemySpawner;
};
