#include "MegatronFunctionLibrary.h"
#include "../Framework/MegatronPlayerController.h"

int32 UMegatronFunctionLibrary::GetPlayerFactionID()
{
	return 0;
}

int32 UMegatronFunctionLibrary::GetEnemyFactionID()
{
	return 1;
}

AMegatronPlayerController* UMegatronFunctionLibrary::GetMegatronPlayerController(UObject* WorldContextObject)
{
	return WorldContextObject ? Cast<AMegatronPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()) : nullptr;
}

