#pragma once

#include "GameFramework/Pawn.h"
#include "Interfaces/HealthInterface.h"
#include "Components/AbilitiesComponent.h"
#include "Megatron/Passives/PassiveBase.h"
#include "Passives/PassiveBase.h"


#include "Slime.generated.h"

class UAbilityBase;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FAbilityUsedSignature, ASlime, OnAbilityUsed, ASlime*, User, UAbilityBase*, Ability, ASlime*, Target);

USTRUCT(BlueprintType)
struct FPassiveAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int damageModifier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int priorityModifier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int targetsToHit;

	//changes who you can target
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int targetFactionOverride;
};

UCLASS(Blueprintable, BlueprintType)
class ASlime : public APawn, public IHealthInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 FactionID = 0;

protected: 
	virtual void OnDamage_Implementation(FDamage Damage) override;

	virtual void OnHeal_Implementation(FDamage Damage) override;

	virtual int32 OnGetHealth_Implementation() override;

	virtual int32 OnGetMaxHealth_Implementation() override;

	virtual float OnGetHealthRatio_Implementation() override;

	virtual void OnAbilityUsedCallback(ASlime* User, UAbilityBase* Ability, ASlime* Target);

	void InitPassives();

	void ActivatePassivesThisTurn();

public:
	ASlime(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	void ForgetRandomAbility();

	UPROPERTY(BlueprintReadOnly)
	bool bHasTurnAvailable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAbilitiesComponent* AbilityComponent;

	UPROPERTY(BlueprintAssignable)
	FAbilityUsedSignature OnAbilityUsed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UAbilityBase>> Abilities;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UPassiveBase>> PassiveClasses;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UPassiveBase*> Passives;
};