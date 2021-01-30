#include "AbilitiesComponent.h"
#include "Abilities/AbilityBase.h"
#include "Abilities/AbilityEmpty.h"
#include "Pawns/Slime.h"



void UAbilitiesComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<UAbilityBase*> UAbilitiesComponent::GetAbilities()
{
	return Abilities;
}

TArray<TSubclassOf<UAbilityBase>> UAbilitiesComponent::GetAbilityClasses()
{
	return AbilityClasses;
}

UAbilityBase* UAbilitiesComponent::GetAbilityAtIndex(int index)
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

TSubclassOf<UAbilityBase> UAbilitiesComponent::GetAbilityClassAtIndex(int index)
{
	if (0 <= index && index < Abilities.Num())
	{
		return AbilityClasses[index]->StaticClass();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability in this slot"));
		return UAbilityEmpty::StaticClass();
	}
}


UAbilitiesComponent::UAbilitiesComponent()
{
	Owner = Cast<ASlime>(GetOwner());
}

void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	for (TSubclassOf<UAbilityBase> AbilityClass : AbilityClasses)
	{
		Abilities.Add(UAbilityBase::InstantiateAbility(AbilityClass, Owner));
	}
}

TSubclassOf<UAbilityBase> UAbilitiesComponent::ForgetAbilityAtIndex(int index)
{
	if (0 <= index && index < Abilities.Num())
	{
		TSubclassOf<UAbilityBase> out = Abilities[index]->StaticClass();
		Abilities[index] = UAbilityBase::InstantiateAbility(UAbilityEmpty::StaticClass(), Owner);
		AbilityClasses[index] = UAbilityEmpty::StaticClass();
		return out;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability in this slot"));
		return UAbilityEmpty::StaticClass();
	}
}

TSubclassOf<UAbilityBase> UAbilitiesComponent::ForgetRandomAbility()
{
	TArray<int> indices;
	for(int i = 0; i < AbilityClasses.Num(); ++i)
	{
		if (AbilityClasses[i] != UAbilityEmpty::StaticClass())
			indices.Add(i);
	}
	if (indices.Num() > 0)
	{
		return ForgetAbilityAtIndex(indices[FMath::RandRange(0, indices.Num() - 1)]);
	}
	return UAbilityEmpty::StaticClass();
}

UAbilityBase* UAbilitiesComponent::LearnNewAbility(int index, TSubclassOf<UAbilityBase> AbilityClass)
{
	for (int i = 0; i < AbilityClasses.Num(); ++i)
	{
		if (AbilityClasses[i] != UAbilityEmpty::StaticClass())
		{
			AbilityClasses[i] = AbilityClass->StaticClass();
			Abilities[i] = UAbilityBase::InstantiateAbility(AbilityClass->StaticClass(), Owner);
			return Abilities[i];
		}	
	}
	return nullptr;
}