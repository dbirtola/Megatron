// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/HealthStatics.h"

void UHealthStatics::Damage(UObject* Target, FDamage Damage)
{
	if (IsValid(Target) && Target->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		IHealthInterface::Execute_OnDamage(Target, Damage);
	}
}

void UHealthStatics::Heal(UObject* Target, FDamage Damage)
{
	if (IsValid(Target) && Target->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		IHealthInterface::Execute_OnHeal(Target, Damage);
	}
}

int32 UHealthStatics::GetHealth(UObject* Target)
{
	if (IsValid(Target) && Target->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		return IHealthInterface::Execute_OnGetHealth(Target);
	}
	else return -1;
}

int32 UHealthStatics::GetMaxHealth(UObject* Target)
{
	if (IsValid(Target) && Target->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		return IHealthInterface::Execute_OnGetMaxHealth(Target);
	}
	else return -1;
}

float UHealthStatics::GetHealthRatio(UObject* Target)
{
	if (IsValid(Target) && Target->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		return IHealthInterface::Execute_OnGetHealthRatio(Target);
	}
	else return -1.f;
}