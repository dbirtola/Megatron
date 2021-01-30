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

bool UAbilityBase::ExecuteAbility_Implementation(ASlime* Target)
{
	return true;
}
