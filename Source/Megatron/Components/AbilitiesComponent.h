#pragma once

#include "Components/ActorComponent.h"
#include "Abilities/AbilityBase.h"
#include "AbilitiesComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UAbilityBase*> AbilityArray;
};