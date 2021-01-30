#include "AbilitiesComponent.h"



void UAbilitiesComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UAbilitiesComponent::UAbilitiesComponent()
{
	Owner = Cast<ASlime>(GetOwner());
}

void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	for (TSubclassOf<UAbilityBase> AbilityClass : Abilities)
	{
		AbilityArray.Add(UAbilityBase::InstantiateAbility(AbilityClass, Owner));
	}
}