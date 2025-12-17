// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseInputComponent.h"

#include "EnhancedInputSubsystems.h"
// #include "Player/BaseLocalPlayer.h"
// #include "Settings/BaseSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseInputComponent)

class UBaseInputConfig;

UBaseInputComponent::UBaseInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UBaseInputComponent::AddInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UBaseInputComponent::RemoveInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UBaseInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
