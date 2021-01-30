// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Framework/MegatronTypes.h"
#include "HealthInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MEGATRON_API IHealthInterface
{
	GENERATED_BODY()

	friend class UHealthStatics;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthInterface")
	void OnDamage(FDamage Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthInterface")
	void OnHeal(FDamage Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthInterface")
	int32 OnGetHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthInterface")
	int32 OnGetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthInterface")
	float OnGetHealthRatio();

	virtual void OnDamage_Implementation(FDamage Damage) { };

	virtual void OnHeal_Implementation(FDamage Damage) { };

	virtual int32 OnGetHealth_Implementation() { return 0; };

	virtual int32 OnGetMaxHealth_Implementation() { return 0; };

	virtual float OnGetHealthRatio_Implementation() { return 0.f; };

};
