#include "AbilitiesComponent.h"

void UAbilitiesComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();
}