// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/HealthInterface.h"
#include "Framework/MegatronTypes.h"
#include "HealthStatics.generated.h"

/**
 * 
 */
UCLASS()
class MEGATRON_API UHealthStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HealthInterface")
		static void Damage(UObject* Target, FDamage Damage);

	UFUNCTION(BlueprintCallable, Category = "HealthInterface")
		static void Heal(UObject* Target, FDamage Damage);

	UFUNCTION(BlueprintCallable, Category = "HealthInterface")
		static int32 GetHealth(UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "HealthInterface")
		static int32 GetMaxHealth(UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "HealthInterface")
		static float GetHealthRatio(UObject* Target);
	
};
