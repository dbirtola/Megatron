// Fill out your copyright notice in the Description page of Project Settings.


#include "Passives/PassiveBase.h"
#include "Abilities/AbilityBase.h"
#include "GameFramework/Actor.h"
#include "Pawns/Slime.h"




UPassive::~UPassive()
{
}

void UPassive::PassiveCreated()
{
	if (PassiveAbility != nullptr) PassiveAbility->Destroy();
	if(PassiveAbilityClass != nullptr)
		PassiveAbility = AAbility::InstantiateAbility(PassiveAbilityClass, Owner);
	OnPassiveApplied();
}

void UPassive::OnPassiveApplied_Implementation()
{
	
}

void UPassive::PassiveTurnTick_Implementation()
{
	if (IsValid(PassiveAbility))
	{
		PassiveAbility->TryExecuteAbility(nullptr);
	}
	TurnTimer--;
	if (TurnTimer <= 0)
	{
		RemovePassive();
	}
}

void UPassive::RemovePassive()
{
	OnPassiveRemoved();
	if (IsValid(PassiveAbility))
		PassiveAbility->Destroy();
}

UPassive * UPassive::InstantiatePassive(TSubclassOf<UPassive> PassiveClass, ASlime * InOwner)
{
	if (*PassiveClass)
	{
		UPassive* out = NewObject<UPassive>(InOwner, *PassiveClass);
		out->Owner = InOwner;
		out->PassiveCreated();
		return out; //later
	}
	return nullptr;
}

void UPassive::OnPassiveRemoved_Implementation()
{

}