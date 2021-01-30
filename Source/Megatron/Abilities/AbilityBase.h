// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MegatronTypes.h"
#include "UObject/NoExportTypes.h"
#include "Pawns/Slime.h"
#include "AbilityBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MEGATRON_API UAbilityBase : public UObject
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadOnly)
	ASlime* Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttribute Attribute = EAttribute::SLIME;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetAbilityName();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetAbilityDescription();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAttribute GetAbilityAttribute();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ExecuteAbility(ASlime* Target);
};
