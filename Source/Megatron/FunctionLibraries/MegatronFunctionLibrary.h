#pragma once

#include "MegatronFunctionLibrary.generated.h"

class AMegatronPlayerController;

UCLASS()
class MEGATRON_API UMegatronFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static int32 GetPlayerFactionID();
	UFUNCTION(BlueprintPure)
	static int32 GetEnemyFactionID();

	UFUNCTION(BlueprintPure)
	static AMegatronPlayerController* GetMegatronPlayerController(UObject* WorldContextObject);
};