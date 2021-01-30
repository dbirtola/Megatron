#pragma once

#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

class UAbilityBase;
class ASlime;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()


	UAbilitiesComponent();

private:
	ASlime* Owner;

	TArray<UAbilityBase*> AbilityArray;

	TArray<TSubclassOf<UAbilityBase>> Abilities;

public:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};