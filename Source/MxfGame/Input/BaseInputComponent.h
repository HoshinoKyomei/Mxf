// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "BaseInputConfig.h"

#include "BaseInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UObject;

/**
 * UBaseInputComponent
 *
 *	Component used to manage input mappings and bindings using an input config data asset.
 */
UCLASS(Blueprintable, Config = Input)
class MXFGAME_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	UBaseInputComponent(const FObjectInitializer& ObjectInitializer);
	
	void AddInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UBaseInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	
	void RemoveBinds(TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
void UBaseInputComponent::BindNativeAction(const UBaseInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UBaseInputComponent::BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FBaseInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}