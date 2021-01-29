// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MegatronGameModeBase.generated.h"


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

	// Generates new enemies for the player to fight.
	void PrepareCombat();

	// Starts combat with enemies present on the field
	void StartCombat();

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



public:
	void Tick(float DeltaSeconds) override;

public:

	// Debug only! Use this 
	UFUNCTION(BlueprintCallable)
	void DEBUG_ForceNextRoundState();

	UPROPERTY(BlueprintReadOnly)
	ERoundState RoundState;

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
};
