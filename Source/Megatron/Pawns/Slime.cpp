#include "Slime.h"
#include "../Components/HealthComponent.h"

void ASlime::OnDamage_Implementation(FDamage Damage)
{
	HealthComponent->TakeDamage(Damage);
}

void ASlime::OnHeal_Implementation(FDamage Damage)
{
	HealthComponent->TakeHealing(Damage);
}

int32 ASlime::OnGetHealth_Implementation()
{
	return HealthComponent->CurrentHealth;
}

int32 ASlime::OnGetMaxHealth_Implementation()
{
	return HealthComponent->MaxHealth;
}

float ASlime::OnGetHealthRatio_Implementation()
{
	return (float) HealthComponent->CurrentHealth / (float) HealthComponent->MaxHealth;
}

ASlime::ASlime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	AbilityComponent = CreateDefaultSubobject<UAbilitiesComponent>("Abilities");
}


void ASlime::BeginPlay()
{
	Super::BeginPlay();
}
