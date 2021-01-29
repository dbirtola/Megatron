
#include "MegatronFunctionLibrary.generated.h"

UCLASS()
class UMegatronFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static int32 GetPlayerFactionID();
	UFUNCTION(BlueprintPure)
	static int32 GetEnemyFactionID();
};