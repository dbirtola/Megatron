#include "Slime.h"
#include "../Components/HealthComponent.h"
#include "Abilities/AbilityBase.h"

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

	OnAbilityUsed.AddDynamic(this, &ASlime::OnAbilityUsedCallback);
}


void ASlime::OnAbilityUsedCallback(ASlime* User, UAbilityBase* Ability, ASlime* Target)
{
	bHasTurnAvailable = false;
	return;
}

void ASlime::ForgetRandomAbility()
{
	// select random ability and forget
	const short value = FMath::RandRange(0, this->Abilities.Num()-1);
	this->Abilities.RemoveAt(value);
}

void ASlime::ActivatePassivesThisTurn()
{
	for(UPassiveBase* Passive : Passives)
	{
		//if value is 0 or -1, then activate
		if(Passive->CurrentTurnTimer <= 0)
		{
			Passive->ActivateAbility();
			
			Passive->CurrentTurnTimer--;
			
			if(Passive->CurrentTurnTimer == 0)
			{
				Passive->CurrentTurnTimer = Passive->TurnTimer;		
			}
		}
	}
}


void ASlime::InitPassives()
{
	for(TSubclassOf<UAbilityBase> Instance : PassiveClasses)
	{
		Passives.Add(NewObject<UPassiveBase>(this, *Instance ));
	}
}

