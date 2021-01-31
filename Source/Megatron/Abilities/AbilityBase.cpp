// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityBase.h"

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
		if (ensure(OwnerSlime))
		{
			OwnerSlime->OnAbilityUsed.Broadcast(OwnerSlime, this, Target);
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
