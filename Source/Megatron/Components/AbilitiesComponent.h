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

	TArray<UAbilityBase*> Abilities;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UAbilityBase>> AbilityClasses;

public:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UAbilityBase*> GetAbilities();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<UAbilityBase>> GetAbilityClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAbilityBase* GetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UAbilityBase> GetAbilityClassAtIndex(int index);


	UFUNCTION(BlueprintCallable)
	TSubclassOf<UAbilityBase> ForgetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UAbilityBase> ForgetRandomAbility();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityClass"))
	UAbilityBase* LearnNewAbility(int index, TSubclassOf<UAbilityBase> AbilityClass);
};