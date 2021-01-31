#pragma once

#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

class AAbility;
class ASlime;
class UPassiveBase;

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
	TArray<UPassiveBase*> Passives;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AAbility>> AbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UPassiveBase>> PassiveClasses;

public:
	TSubclassOf<AAbility> LastUsedAbilityClass;

	void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AAbility*> GetAbilities();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPassiveBase*> GetPassives();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<AAbility>> GetAbilityClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<TSubclassOf<UPassiveBase>> GetPassiveClasses();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAbility* GetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPassiveBase* GetPassiveAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AAbility> GetAbilityClassAtIndex(int index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UPassiveBase> GetPassiveClassAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetAbilityAtIndex(int index);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAbility> ForgetRandomAbility();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityClass"))
	AAbility* LearnNewAbility(TSubclassOf<AAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "PassiveClass"))
	UPassiveBase* GainPassive(TSubclassOf<UPassiveBase> PassiveClass);

	UFUNCTION(BlueprintCallable)
	void LosePassive(UPassiveBase* PassiveToLose);

};