#include "GameFramework/PlayerController.h"
#include "MegatronPlayerController.generated.h"



UCLASS(Blueprintable, BlueprintType)
class AMegatronPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

};