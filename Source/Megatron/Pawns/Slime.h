#include "GameFramework/Pawn.h"
#include "Interfaces/HealthInterface.h"
#include "Components/AbilitiesComponent.h"

#include "Slime.generated.h"

class UHealthComponent;

UCLASS(Blueprintable, BlueprintType)
class ASlime : public APawn, public IHealthInterface
{
	GENERATED_BODY()

	int32 FactionID = 0;

protected: 
	virtual void OnDamage_Implementation(FDamage Damage) override;

	virtual void OnHeal_Implementation(FDamage Damage) override;

	virtual int32 OnGetHealth_Implementation() override;

	virtual int32 OnGetMaxHealth_Implementation() override;

	virtual float OnGetHealthRatio_Implementation() override;

public:
	ASlime(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAbilitiesComponent* AbilityComponent;
};