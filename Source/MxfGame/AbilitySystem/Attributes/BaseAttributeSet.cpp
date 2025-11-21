// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BaseAttributeSet.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"

class UWorld;

UBaseAttributeSet::UBaseAttributeSet()
{
}

UWorld* UBaseAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UBaseAbilitySystemComponent* UBaseAttributeSet::GetBaseAbilitySystemComponent() const
{
	return Cast<UBaseAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

UAbilitySystemComponent* UBaseAttributeSet::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
}