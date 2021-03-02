#include "Slime.h"
#include "Components/HealthComponent.h"
#include "Components/AbilitiesComponent.h"
#include "Abilities/AbilityBase.h"
#include "Passives/PassiveBase.h"
#include "Framework/MegatronTypes.h"

void ASlime::OnDamage_Implementation(FDamage Damage)
{
	for(UPassive* Passive : GetPassives())
	{
		Damage.BaseDamage += Passive->PassiveSets.AttackDamageModifier;
		Damage.BaseDamage -= Passive->PassiveSets.DefenseDamageModifiers;
	}
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

TArray<UPassive*> ASlime::GetPassives()
{
	return AbilityComponent->GetPassives();
}

TArray<TSubclassOf<AAbility>> ASlime::GetAbilityClasses()
{
	return AbilityComponent->GetAbilityClasses();
}

TArray<TSubclassOf<UPassive>> ASlime::GetPassiveClasses()
{
	return AbilityComponent->GetPassiveClasses();
}

AAbility* ASlime::GetAbilityAtIndex(int index)
{
	return AbilityComponent->GetAbilityAtIndex(index);
}

UPassive * ASlime::GetPassiveAtIndex(int index)
{
	return AbilityComponent->GetPassiveAtIndex(index);
}


TSubclassOf<AAbility> ASlime::GetAbilityClassAtIndex(int index)
{
	return AbilityComponent->GetAbilityClassAtIndex(index);
}

TSubclassOf<UPassive> ASlime::GetPassiveClassAtIndex(int index)
{
	return AbilityComponent->GetPassiveClassAtIndex(index);
}

TSubclassOf<AAbility>  ASlime::ForgetAbilityAtIndex(int index) 
{
	return AbilityComponent->ForgetAbilityAtIndex(index);
}

TSubclassOf<AAbility> ASlime::ForgetAbilityByReference(AAbility* reference)
{
	return AbilityComponent->ForgetAbilityByReference(reference);
}

TSubclassOf<AAbility>  ASlime::ForgetRandomAbility()
{
	TSubclassOf<AAbility> out = AbilityComponent->ForgetRandomAbility();
	OnForgotAbility();
	return out;
}

AAbility*  ASlime::LearnNewAbility(TSubclassOf<AAbility> AbilityClass)
{
	return AbilityComponent->LearnNewAbility(AbilityClass);
}

UPassive * ASlime::GainPassive(TSubclassOf<UPassive> PassiveClass)
{
	return AbilityComponent->GainPassive(PassiveClass);
}

void ASlime::LosePassive(UPassive * PassiveToLose)
{
	return AbilityComponent->LosePassive(PassiveToLose);
}

TSubclassOf<AAbility> ASlime::GetLastUsedAbilityClass()
{
	return AbilityComponent->LastUsedAbilityClass;
}
