#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetHealth();
}

void UHealthComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(FDamage Damage)
{
	bool bIsHeal = Damage.BaseDamage < 0;

	// TODO: Take any modifiers into account.
	int32 FinalDamage = Damage.BaseDamage;
	CurrentHealth -= FinalDamage;

	FString DamageCauserName("An Unknown Source");
	if (ensureMsgf(Damage.Instigator, TEXT("For debugging purposes, a valid damage causer is required for all sources of damage taken. Ensure the damage causer isn't being destroyed before it deals its damage")))
	{
		DamageCauserName = Damage.Instigator->GetName();
	}
	UE_LOG(LogTemp, Log, TEXT("%s took %d damage from %s"), *GetOwner()->GetName(), FinalDamage, *DamageCauserName);


	// Make sure we don't have more health than our maximum.
	// TODO: If abilities can grant more max health, we should check against an "EffectiveMaxHealth" value instead of their base MaxHealth.
	if (CurrentHealth > MaxHealth)
	{
		ResetHealth();
	}

	// Make sure to announce damage was taken before we announce if something has died or not.
	OnTakeDamage.Broadcast(GetOwner(), Damage.Instigator, Damage);

	if (CurrentHealth <= 0)
	{
		Die(Damage.Instigator);
	}
}

void UHealthComponent::TakeHealing(FDamage Damage)
{
	Damage.BaseDamage *= -1;
	TakeDamage(Damage);
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
}

void UHealthComponent::Die(AActor* DamageCauser)
{
	CurrentHealth = 0;
	OnDeath.Broadcast(GetOwner(), DamageCauser);
}

