// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityBase.h"
#include "AbilityEmpty.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MEGATRON_API AAbilityEmpty : public AAbility
{
	GENERATED_BODY()

	AAbilityEmpty();

	ETargetType TargetType = ETargetType::ENEMY;

	virtual void ExecuteAbility_Implementation(ASlime* Target) override;
};
