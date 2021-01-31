#include "Slime.h"
#include "Components/HealthComponent.h"
#include "Components/AbilitiesComponent.h"
#include "Abilities/AbilityBase.h"
#include "Framework/MegatronTypes.h"

void ASlime::OnDamage_Implementation(FDamage Damage)
{
	if (Damage.BaseDamage < 1) Damage.BaseDamage = 1;
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

	OnAbilityUsed.AddDynamic(this, &ASlime::OnAbilityUsedCallback);
}


void ASlime::OnAbilityUsedCallback(ASlime* User, AAbility* Ability, ASlime* Target)
{
	if(Ability->GetAbilityTargetType() != ETargetType::PASSIVE)
		bHasTurnAvailable = false;
	return;
}

TArray<AAbility*> ASlime::GetAbilities()
{
	return AbilityComponent->GetAbilities();
}

TArray<TSubclassOf<AAbility>> ASlime::GetAbilityClasses()
{
	return AbilityComponent->GetAbilityClasses();
}

AAbility* ASlime::GetAbilityAtIndex(int index)
{
	return AbilityComponent->GetAbilityAtIndex(index);
}


TSubclassOf<AAbility> ASlime::GetAbilityClassAtIndex(int index)
{
	return AbilityComponent->GetAbilityClassAtIndex(index);
}

void ASlime::ForgetRandomAbility()
{
	AbilityComponent->ForgetRandomAbility();
}