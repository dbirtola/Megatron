// Fill out your copyright notice in the Description page of Project Settings.


#include "Passives/PassiveBase.h"
#include "Abilities/AbilityBase.h"
#include "GameFramework/Actor.h"
#include "Pawns/Slime.h"




UPassiveBase::~UPassiveBase()
{
}

void UPassiveBase::PassiveCreated()
{
	if (PassiveAbility != nullptr) PassiveAbility->Destroy();
	if(PassiveAbilityClass != nullptr)
		PassiveAbility = AAbility::InstantiateAbility(PassiveAbilityClass, Owner);
	OnPassiveApplied();
}

void UPassiveBase::OnPassiveApplied_Implementation()
{
	
}

void UPassiveBase::PassiveTurnTick_Implementation()
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

void UPassiveBase::RemovePassive()
{
	OnPassiveRemoved();
	if (IsValid(PassiveAbility))
		PassiveAbility->Destroy();
}

UPassiveBase * UPassiveBase::InstantiatePassive(TSubclassOf<UPassiveBase> PassiveClass, ASlime * InOwner)
{
	if (*PassiveClass)
	{
		UPassiveBase* out = NewObject<UPassiveBase>(InOwner, *PassiveClass);
		out->Owner = InOwner;
		out->PassiveCreated();
		return out; //later
	}
	return nullptr;
}

void UPassiveBase::OnPassiveRemoved_Implementation()
{

}