// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityBase.h"
#include "Abilities/AbilityEmpty.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/DefaultLevelSequenceInstanceData.h"
#include "LevelSequence/Public/LevelSequenceActor.h"

void AAbility::PlayLevelSequence(ULevelSequencePlayer* LevelSequencePlayer, ALevelSequenceActor* LevelSequenceActor, ULevelSequence* LevelSequence, AActor* TransformOriginActor)
{
	FMovieSceneSequencePlaybackSettings Settings;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, LevelSequenceActor);
	LevelSequenceActor->bOverrideInstanceData = true;
	UDefaultLevelSequenceInstanceData* DefaultLevelSequenceInstanceData = Cast<UDefaultLevelSequenceInstanceData>(LevelSequenceActor->DefaultInstanceData);
	DefaultLevelSequenceInstanceData->TransformOriginActor = TransformOriginActor;
	LevelSequencePlayer->Play();
}

ASlime* AAbility::GetOwningSlime()
{
	return OwnerSlime;
}

FName AAbility::GetAbilityName()
{
	return Name;
}

FText AAbility::GetAbilityDescription()
{
	return Description;
}

EAttribute AAbility::GetAbilityAttribute()
{
	return Attribute;
}

ETargetType AAbility::GetAbilityTargetType()
{
	return TargetType;
}

void AAbility::SetOwnerSlime(ASlime * NewOwner)
{
	OwnerSlime = NewOwner;
}

bool AAbility::CanExecuteAbility() const
{
	if (ensure(OwnerSlime))
	{
		return OwnerSlime->bHasTurnAvailable;
	}
	return false;
}

bool AAbility::TryExecuteAbility(ASlime* Target)
{
	if (CanExecuteAbility())
	{
		ExecuteAbility(Target);


		if (IsValid(OwnerSlime))
		{

			// TODO: Do these need to be cleaned up?
			if (SelfLevelSequence)
			{
				PlayLevelSequence(SelfLevelSequencePlayer, SelfLevelSequenceActor, SelfLevelSequence, OwnerSlime);
			}
			if (TargetLevelSequence)
			{
				PlayLevelSequence(TargetLevelSequencePlayer, TargetLevelSequenceActor, TargetLevelSequence, Target);
			}

			if (TargetType != ETargetType::PASSIVE && !IsA(AAbilityEmpty::StaticClass()))
			{
				OwnerSlime->AbilityComponent->LastUsedAbilityClass = GetClass();
				OwnerSlime->OnAbilityUsed.Broadcast(OwnerSlime, this, Target);
			}
			return true;
		}
	}
	return false;
}

AAbility * AAbility::InstantiateAbility(TSubclassOf<AAbility> AbilityClass, ASlime* InOwner)
{
	
	AAbility* out = InOwner->GetWorld()->SpawnActor<AAbility>(*AbilityClass);
	out->SetOwnerSlime(InOwner);
	return out; //later
}

void AAbility::ExecuteAbility_Implementation(ASlime* Target)
{
}

void AAbility::OnAbilityFinished()
{
}
