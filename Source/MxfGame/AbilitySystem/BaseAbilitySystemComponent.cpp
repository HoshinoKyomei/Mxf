// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAbilitySystemComponent)

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
