// Copyright 2019 Sony Interactive Entertainment, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"

#include "MegatronTypes.generated.h"

UENUM(BlueprintType)
enum class BuildConfig : uint8
{
	SHIPPING,
	DEVELOPMENT,
	EDITOR
};
