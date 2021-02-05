// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MegatronTypes.h"
#include "UObject/NoExportTypes.h"
#include "Pawns/Slime.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "AbilityBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MEGATRON_API AAbility : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadOnly)
	ASlime* OwnerSlime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttribute Attribute = EAttribute::SLIME;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETargetType TargetType = ETargetType::ENEMY;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* SelfEmitterTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* TargetEmitterTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelSequence* SelfLevelSequence;

	UPROPERTY()
	ULevelSequencePlayer* SelfLevelSequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* SelfLevelSequenceActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelSequence* TargetLevelSequence;

	UPROPERTY()
	ULevelSequencePlayer* TargetLevelSequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* TargetLevelSequenceActor;


	UFUNCTION()
	void PlayLevelSequence(ULevelSequencePlayer* LevelSequencePlayer, ALevelSequenceActor* LevelSequenceActor, ULevelSequence* LevelSequence, AActor* TransformOriginActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASlime* GetOwningSlime();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetAbilityName();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetAbilityDescription();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAttribute GetAbilityAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETargetType GetAbilityTargetType();

	UFUNCTION(BlueprintCallable)
	void SetOwnerSlime(ASlime* NewOwner);

	bool CanExecuteAbility() const;

	// Checks if the owning slime still has a turn available and executes the ability if possible.
	UFUNCTION(BlueprintCallable)
	bool TryExecuteAbility(ASlime* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteAbility(ASlime* Target);

	UFUNCTION(BlueprintCallable)
	void OnAbilityFinished();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "AbilityClass"))
	static AAbility* InstantiateAbility(TSubclassOf<AAbility> AbilityClass, ASlime* InOwner);
};
