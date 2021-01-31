// Fill out your copyright notice in the Description page of Project Settings.


#include "Passives/PassiveBase.h"
#include "Abilities/AbilityBase.h"
#include "GameFramework/Actor.h"
#include "Pawns/Slime.h"




UPassiveBase::~UPassiveBase()
{
	if(IsValid(PassiveAbility))
		PassiveAbility->Destroy();
}

void UPassiveBase::PassiveCreated()
{
	if (PassiveAbility != nullptr) PassiveAbility->Destroy();
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
}

UPassiveBase * UPassiveBase::InstantiatePassive(TSubclassOf<UPassiveBase> PassiveClass, ASlime * InOwner)
{
	UPassiveBase* out = NewObject<UPassiveBase>(InOwner, *PassiveClass);
	out->Owner = InOwner;
	out->PassiveCreated();
	return out; //later
}

void UPassiveBase::OnPassiveRemoved_Implementation()
{

}