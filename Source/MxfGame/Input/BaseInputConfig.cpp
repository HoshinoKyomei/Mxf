// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseInputConfig.h"

#include "BaseLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseInputConfig)


UBaseInputConfig::UBaseInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UBaseInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FBaseInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBase, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UBaseInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FBaseInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBase, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
