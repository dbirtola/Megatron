// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityEmpty.h"

AAbilityEmpty::AAbilityEmpty()
{
	Name = FName(TEXT("Empty"));
	Description = FText::FromString("Empty Ability Slot");
}

void AAbilityEmpty::ExecuteAbility_Implementation(ASlime* Target)
{
	OwnerSlime->LearnNewAbility(Target->GetLastUsedAbilityClass());
}