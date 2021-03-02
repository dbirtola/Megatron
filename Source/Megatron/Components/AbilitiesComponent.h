#pragma once

#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

class AAbility;
class ASlime;
class UPassive;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()


	UAbilitiesComponent();

private:
	ASlime* Owner;

	UPROPERTY()
	TArray<AAbility*> Abilities;
	
	UPROPERTY()
	TArray<UPassive*> Passives;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AAbility>> AbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UPassive>> PassiveClasses;

public:
	TSubclassOf<AAbility> LastUsedAbilityClass;

	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AAbility*> GetAbilities();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPassive*> GetPassives();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<AAbility>> GetAbilityClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<UPassive>> GetPassiveClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAbility* GetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPassive* GetPassiveAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AAbility> GetAbilityClassAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UPassive> GetPassiveClassAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetAbilityByReference(AAbility* reference);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetRandomAbility();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityClass"))
	AAbility* LearnNewAbility(TSubclassOf<AAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "PassiveClass"))
	UPassive* GainPassive(TSubclassOf<UPassive> PassiveClass);

	UFUNCTION(BlueprintCallable)
	void LosePassive(UPassive* PassiveToLose);

};