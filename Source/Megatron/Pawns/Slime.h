#include "GameFramework/Pawn.h"
#include "Slime.generated.h"

class UHealthComponent;

UCLASS(Blueprintable, BlueprintType)
class ASlime : public APawn
{
	GENERATED_BODY()

	int32 FactionID = 0;
public:
	ASlime(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHealthComponent* HealthComponent;
};