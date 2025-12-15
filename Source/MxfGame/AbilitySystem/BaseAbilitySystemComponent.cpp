// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAbilitySystemComponent)

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	K2_ConstructionScript();
}

void UBaseAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UBaseAbilitySystemComponent::K2_SetNumericAttributeBase(const FGameplayAttribute& Attribute, float NewBaseValue)
{
	SetNumericAttributeBase(Attribute, NewBaseValue);
}

void UBaseAbilitySystemComponent::K2_SetLooseGameplayTagCount(const FGameplayTag& GameplayTag, int32 NewCount)
{
	SetLooseGameplayTagCount(GameplayTag, NewCount);
}

void UBaseAbilitySystemComponent::K2_CancelAbilities(const FGameplayTagContainer WithTags,
                                                     FGameplayTagContainer WithoutTags, UGameplayAbility* Ignore)
{
	CancelAbilities(&WithTags, &WithoutTags, Ignore);
}

void UBaseAbilitySystemComponent::K2_RemoveAllGameplayCues()
{
	RemoveAllGameplayCues();
}

void UBaseAbilitySystemComponent::K2_SetAvatarActor(AActor* InAvatarActor)
{
	SetAvatarActor(InAvatarActor);
}

void UBaseAbilitySystemComponent::K2_ApplyModToAttribute(const FGameplayAttribute& Attribute,
	TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude)
{
	ApplyModToAttribute(Attribute, ModifierOp, ModifierMagnitude);
}

void UBaseAbilitySystemComponent::K2_ClearActorInfo()
{
	ClearActorInfo();
}

void UBaseAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				K2_PressedAbilitySpec(AbilitySpec.Handle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				K2_ReleasedAbilitySpec(AbilitySpec.Handle);
			}
		}
	}
}
