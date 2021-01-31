#pragma once

#include "GameFramework/Pawn.h"
#include "Interfaces/HealthInterface.h"
#include "Components/AbilitiesComponent.h"

#include "Slime.generated.h"

class AAbility;
class UHealthComponent;
class UAbilitiesComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FAbilityUsedSignature, ASlime, OnAbilityUsed, ASlime*, User, AAbility*, Ability, ASlime*, Target);


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

	UFUNCTION()
	virtual void OnAbilityUsedCallback(ASlime* User, AAbility* Ability, ASlime* Target);

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AAbility*> GetAbilities();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<AAbility>> GetAbilityClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAbility* GetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AAbility> GetAbilityClassAtIndex(int index);
};