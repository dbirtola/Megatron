#pragma once

#include "GameFramework/Pawn.h"
#include "Interfaces/HealthInterface.h"
#include "Components/AbilitiesComponent.h"

#include "Slime.generated.h"

class UAbilityBase;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FAbilityUsedSignature, ASlime, OnAbilityUsed, ASlime*, User, UAbilityBase*, Ability, ASlime*, Target);


UCLASS(Blueprintable, BlueprintType)
class ASlime : public APawn, public IHealthInterface
{
	GENERATED_BODY()


protected: 
	virtual void OnDamage_Implementation(FDamage Damage) override;

	virtual void OnHeal_Implementation(FDamage Damage) override;

	virtual int32 OnGetHealth_Implementation() override;

	virtual int32 OnGetMaxHealth_Implementation() override;

	virtual float OnGetHealthRatio_Implementation() override;

	virtual void OnAbilityUsedCallback(ASlime* User, UAbilityBase* Ability, ASlime* Target);

public:
	ASlime(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	void ForgetRandomAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere);
	int32 FactionID = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bHasTurnAvailable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAbilitiesComponent* AbilityComponent;

	UPROPERTY(BlueprintAssignable)
	FAbilityUsedSignature OnAbilityUsed;
};