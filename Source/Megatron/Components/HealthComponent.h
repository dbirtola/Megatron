#pragma once

#include "Components/ActorComponent.h"
#include "Framework/MegatronTypes.h"
#include "HealthComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FTakeDamageSignature, UHealthComponent, OnTakeDamage, AActor*, DamagedActor, AActor*, DamageCauser, FDamage, Damage);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FDeathSignature, UHealthComponent, OnDeath, AActor*, DyingActor, AActor*, DamageCauser);


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Deals a set amount of damage to the entity. Negative damage will heal the entity instead.
	UFUNCTION(BlueprintCallable)
	void TakeDamage(FDamage Damage);

	UFUNCTION(BlueprintCallable)
	void TakeHealing(FDamage Damage);


private:
	void ResetHealth();
	void Die(AActor*DamageCauser);

public:
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ClampMin="0"))
	int32 MaxHealth = 10;

	/** Called when the actor is damaged in any way. */
	UPROPERTY(BlueprintAssignable, Category = "Game|Damage")
	FTakeDamageSignature OnTakeDamage;

	/** Called when the actor is damaged in any way. */
	UPROPERTY(BlueprintAssignable, Category = "Game|Damage")
	FDeathSignature OnDeath;
};