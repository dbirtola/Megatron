#pragma once

#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

class AAbility;
class ASlime;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()


	UAbilitiesComponent();

private:
	ASlime* Owner;

	TArray<AAbility*> Abilities;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AAbility>> AbilityClasses;

public:
	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AAbility*> GetAbilities();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<AAbility>> GetAbilityClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAbility* GetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AAbility> GetAbilityClassAtIndex(int index);


	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetRandomAbility();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityClass"))
	AAbility* LearnNewAbility(int index, TSubclassOf<AAbility> AbilityClass);
};