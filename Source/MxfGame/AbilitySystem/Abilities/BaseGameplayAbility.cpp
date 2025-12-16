// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BaseGameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseGameplayAbility)

UBaseGameplayAbility::UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EBaseAbilityActivationPolicy::OnInputTriggered;
}
