// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PassiveBase.generated.h"

class ASlime;
class AAbility;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPassiveStats
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offense")
		int AttackDamageModifier;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offense")
	//	int AttackPriorityModifier;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offense")
	//	int TargetsToHit;

	////changes who you can target
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Offense")
	//	int TargetFactionOverride;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
		int DefenseDamageModifiers;
};

UCLASS(BlueprintType, Blueprintable)
class MEGATRON_API UPassive : public UObject
{
	GENERATED_BODY()

	ASlime* Owner;

	AAbility* PassiveAbility;

	void PassiveCreated();

	~UPassive();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAbility> PassiveAbilityClass;

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnPassiveApplied();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PassiveTurnTick();

	UFUNCTION(BlueprintNativeEvent)
	void OnPassiveRemoved();

	UFUNCTION(BlueprintCallable)
	void RemovePassive();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPassiveStats PassiveSets;
	
	//how many turns between activations. If -1, should activate each turn
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TurnTimer{5};

	////
	//UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//int CurrentTurnTimer;

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "PassiveClass"))
	static UPassive* InstantiatePassive(TSubclassOf<UPassive> PassiveClass, ASlime* InOwner);
};
