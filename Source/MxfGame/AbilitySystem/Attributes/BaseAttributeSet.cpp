// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseAttributeSet.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAttributeSet)

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

