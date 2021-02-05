#pragma once

#include "MegatronFunctionLibrary.generated.h"

class AMegatronPlayerController;
class AMegatronGameModeBase;

UCLASS()
class MEGATRON_API UMegatronFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static int32 GetPlayerFactionID();
	UFUNCTION(BlueprintPure)
	static int32 GetEnemyFactionID();

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static AMegatronPlayerController* GetMegatronPlayerController(UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, meta= (WorldContext = "WorldContextObject"))
	static AMegatronGameModeBase* GetMegatronGameMode(UObject* WorldContextObject);
};