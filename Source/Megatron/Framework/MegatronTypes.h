// Copyright 2019 Sony Interactive Entertainment, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"

#include "MegatronTypes.generated.h"

UENUM(BlueprintType)
enum class EBuildConfig : uint8
{
	SHIPPING,
	DEVELOPMENT,
	EDITOR
};

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	SLIME, 
	ACID,
	WATER
};

USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator;
		
	UPROPERTY(BlueprintReadWrite)
	int32 BaseDamage;

	UPROPERTY(BlueprintReadWrite)
	EAttribute Attribute;

	UPROPERTY(BlueprintReadWrite)
	FString DebugInfo;
};