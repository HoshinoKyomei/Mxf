// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseCombatSet.h"

#include "AbilitySystem/Attributes/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

UBaseCombatSet::UBaseCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UBaseCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UBaseCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCombatSet, BaseDamage, OldValue);
}

void UBaseCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCombatSet, BaseHeal, OldValue);
}