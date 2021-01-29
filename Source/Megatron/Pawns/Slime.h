#include "GameFramework/Pawn.h"
#include "Slime.generated.h"

class UHealthComponent;

UCLASS(Blueprintable, BlueprintType)
class ASlime : public APawn
{
	GENERATED_BODY()

public:
	ASlime(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHealthComponent* HealthComponent;
};