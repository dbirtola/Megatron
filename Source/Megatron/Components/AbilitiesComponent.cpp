#include "AbilitiesComponent.h"
#include "Abilities/AbilityBase.h"
#include "Abilities/AbilityEmpty.h"
#include "Passives/PassiveBase.h"
#include "Pawns/Slime.h"




UAbilitiesComponent::UAbilitiesComponent()
{
	Owner = Cast<ASlime>(GetOwner());
}

void UAbilitiesComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<AAbility*> UAbilitiesComponent::GetAbilities()
{
	return Abilities;
}


TArray<UPassive*> UAbilitiesComponent::GetPassives()
{
	return Passives;
}

TArray<TSubclassOf<AAbility>> UAbilitiesComponent::GetAbilityClasses()
{
	return AbilityClasses;
}

TArray<TSubclassOf<UPassive>> UAbilitiesComponent::GetPassiveClasses()
{
	return PassiveClasses;
}

AAbility* UAbilitiesComponent::GetAbilityAtIndex(int index)
{
	if (0 <= index && index < Abilities.Num())
	{
		return Abilities[index];
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability in this slot"));
		return nullptr;
	}
}

UPassive* UAbilitiesComponent::GetPassiveAtIndex(int index)
{
	if (0 <= index && index < Passives.Num())
	{
		return Passives[index];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No passive in this slot"));
		return nullptr;
	}
}

TSubclassOf<AAbility> UAbilitiesComponent::GetAbilityClassAtIndex(int index)
{
	if (0 <= index && index < Abilities.Num())
	{
		return *AbilityClasses[index];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability in this slot"));
		return AAbilityEmpty::StaticClass();
	}
}

TSubclassOf<UPassive> UAbilitiesComponent::GetPassiveClassAtIndex(int index)
{
	if (0 <= index && index < Passives.Num())
	{
		return *PassiveClasses[index];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No passive in this slot"));
		return nullptr;
	}
}


void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	for (TSubclassOf<AAbility> AbilityClass : AbilityClasses)
	{
		Abilities.Add(AAbility::InstantiateAbility(AbilityClass, Owner));
	}

	for (TSubclassOf<UPassive> PassiveClass : PassiveClasses)
	{
		if (*PassiveClass)
		{
			Passives.Add(UPassive::InstantiatePassive(PassiveClass, Owner));
		}
	}
}

TSubclassOf<AAbility> UAbilitiesComponent::ForgetAbilityAtIndex(int index)
{
	if (0 <= index && index < Abilities.Num())
	{
		TSubclassOf<AAbility> out = Abilities[index]->StaticClass();
		Abilities[index] = AAbility::InstantiateAbility(AAbilityEmpty::StaticClass(), Owner);
		AbilityClasses[index] = AAbilityEmpty::StaticClass();
		return out;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability in this slot"));
		return AAbilityEmpty::StaticClass();
	}
}

TSubclassOf<AAbility> UAbilitiesComponent::ForgetAbilityByReference(AAbility* reference)
{
	if(Abilities.Remove(reference) > 0)
		return reference->StaticClass();
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability was not in array"));
		return AAbilityEmpty::StaticClass();
	}
}

TSubclassOf<AAbility> UAbilitiesComponent::ForgetRandomAbility()
{
	TArray<int> indices;
	for(int i = 0; i < AbilityClasses.Num(); ++i)
	{
		if (AbilityClasses[i] != AAbilityEmpty::StaticClass())
			indices.Add(i);
	}
	if (indices.Num() > 0)
	{
		return ForgetAbilityAtIndex(indices[FMath::RandRange(0, indices.Num() - 1)]);
	}
	return AAbilityEmpty::StaticClass();
}

AAbility* UAbilitiesComponent::LearnNewAbility(TSubclassOf<AAbility> AbilityClass)
{
	for (int i = 0; i < AbilityClasses.Num(); ++i)
	{
		if (AbilityClasses[i] == AAbilityEmpty::StaticClass())
		{
			AbilityClasses[i] = *AbilityClass;
			Abilities[i] = AAbility::InstantiateAbility(*AbilityClass, Owner);
			Owner->OnLearnedAbility();
			return Abilities[i];
		}	
	}
	return nullptr;
}

UPassive* UAbilitiesComponent::GainPassive(TSubclassOf<UPassive> PassiveClass)
{
	UPassive* out = UPassive::InstantiatePassive(*PassiveClass, Owner);
	PassiveClasses.Add(PassiveClass);
	Passives.Add(out);
	return out;
}

void UAbilitiesComponent::LosePassive(UPassive* PassiveToLose)
{
	int index = Passives.Find(PassiveToLose);
	if (index != INDEX_NONE)
	{
		Passives.RemoveAt(index);
		PassiveClasses.RemoveAt(index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PassiveToLose not found"));
	}
}