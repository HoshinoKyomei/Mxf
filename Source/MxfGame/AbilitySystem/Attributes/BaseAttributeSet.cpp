// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAttributeSet)

class UWorld;


UBaseAttributeSet::UBaseAttributeSet()
{
	K2_ConstructionScript();
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

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	K2_PreAttributeChange(Attribute, NewValue);
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	K2_PostAttributeChange(Attribute, OldValue, NewValue);
}

FGameplayTagContainer UBaseAttributeSet::K2_GetDynamicAssetTags(const FGameplayEffectSpec& InEffectSpec) const
{
	return InEffectSpec.GetDynamicAssetTags();
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewBaseValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewBaseValue);

	K2_PreAttributeBaseChange(Attribute, NewBaseValue);
}

bool UBaseAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	FBaseGameplayModifierEvaluatedData InData(
		Data.EvaluatedData.Attribute,
		Data.EvaluatedData.ModifierOp,
		Data.EvaluatedData.Magnitude,
		Data.EvaluatedData.Handle
	);

	if (K2_PreGameplayEffectExecute(Data.EffectSpec, InData, &Data.Target))
	{
		// If the blueprint event returns true, update the original data with any modifications
		Data.EvaluatedData.Attribute = InData.Attribute;
		Data.EvaluatedData.ModifierOp = InData.ModifierOp;
		Data.EvaluatedData.Magnitude = InData.Magnitude;
		return true;
	}

	return false;
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FBaseGameplayModifierEvaluatedData InData(
		Data.EvaluatedData.Attribute,
		Data.EvaluatedData.ModifierOp,
		Data.EvaluatedData.Magnitude,
		Data.EvaluatedData.Handle
	);
	
	K2_PostGameplayEffectExecute(Data.EffectSpec, InData, &Data.Target);
}

bool UBaseAttributeSet::K2_PreGameplayEffectExecute_Implementation(const FGameplayEffectSpec& InEffectSpec,
	FBaseGameplayModifierEvaluatedData& InEvaluatedData, UAbilitySystemComponent* InTarget)
{
	// Default native implementation. If you don't override this in Blueprint, this will be called.
	// You can add default clamping logic here if you want.
	return false;
}

bool UBaseAttributeSet::K2_PreAttributeBaseChange_Implementation(const FGameplayAttribute& Attribute,
	float& NewBaseValue) const
{
	// Default native implementation. If you don't override this in Blueprint, this will be called.
	// You can add default clamping logic here if you want.
	return false;
}


bool UBaseAttributeSet::K2_PreAttributeChange_Implementation(const FGameplayAttribute& Attribute, float& NewValue)
{
	// Default native implementation. If you don't override this in Blueprint, this will be called.
	// You can add default clamping logic here if you want.
	return false;
}
