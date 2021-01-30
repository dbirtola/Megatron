// Copyright Epic Games, Inc. All Rights Reserved.


#include "MegatronGameModeBase.h"

void AMegatronGameModeBase::PrepareCombat()
{

}

void AMegatronGameModeBase::StartCombat()
{

}

void AMegatronGameModeBase::TickCombat()
{
	switch (RoundState)
	{
	case ERoundState::NOT_STARTED:
		StartCombat();
		break;
	case ERoundState::PLAYER_TURN:
		if (false /*Check if player turn is over*/)
		{
			FinishPlayerTurn();
		}
		break;
	case ERoundState::ENEMY_TURN:
		if (false /*Check if enemy turn is over*/)
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
}

void AMegatronGameModeBase::FinishForgetAbilitySegment()
{
	RoundState = ERoundState::FINISHED;
}

void AMegatronGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickCombat();
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
