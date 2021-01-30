// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityBase.h"

FName UAbilityBase::GetAbilityName()
{
	return Name;
}

FText UAbilityBase::GetAbilityDescription()
{
	return Description;
}

EAttribute UAbilityBase::GetAbilityAttribute()
{
	return Attribute;
}

void UAbilityBase::SetOwner(ASlime * NewOwner)
{
	Owner = NewOwner;
}

bool UAbilityBase::CanExecuteAbility() const
{
	if (ensure(Owner))
	{
		return Owner->bHasTurnAvailable;
	}
	return false;
}

bool UAbilityBase::TryExecuteAbility(ASlime* Target)
{
	if (CanExecuteAbility())
	{
		ExecuteAbility(Target);
		if (ensure(Owner))
		{
			Owner->OnAbilityUsed.Broadcast(Owner, this, Target);
			return true;
		}
	}
	return false;
}

bool UAbilityBase::ExecuteAbility_Implementation(ASlime* Target)
{
	return true;
}
