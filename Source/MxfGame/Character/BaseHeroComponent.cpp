// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseHeroComponent.h"

#include "BaseGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/BaseInputComponent.h"

const FName UBaseHeroComponent::NAME_BindInputsNow("BindInputsNow");

UBaseHeroComponent::UBaseHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBaseHeroComponent::BindAbilityInput(const APawn* Pawn, const UBaseInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;
	
	UBaseInputComponent* BaseIC = Pawn->FindComponentByClass<UBaseInputComponent>();
	if (ensureMsgf(BaseIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULyraInputComponent or a subclass of it.")))
	{
		BaseIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
}

void UBaseHeroComponent::SendBindInputsEvent(const APawn* Pawn, const APlayerController* PC)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UBaseHeroComponent::SetupInputBind(UInputComponent* PlayerInputComponent, UEnhancedInputLocalPlayerSubsystem* Subsystem, const UBaseInputConfig* InputConfig)
{
	// The Base Input Component has some additional functions to map Gameplay Tags to an Input Action.
	// If you want this functionality but still want to change your input component class, make it a subclass
	// of the UBaseInputComponent or modify this component accordingly.
	UBaseInputComponent* BaseIC = Cast<UBaseInputComponent>(PlayerInputComponent);
	if (ensureMsgf(BaseIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UBaseInputComponent or a subclass of it.")))
	{
		// Add the key mappings that may have been set by the player
		BaseIC->AddInputMappings(InputConfig, Subsystem);

		// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
		// be triggered directly by these input actions Triggered events. 
		TArray<uint32> BindHandles;
		BaseIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
		BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
		BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
		BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
		BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
	}
}

void UBaseHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	K2_Input_AbilityInputTagPressed(InputTag);
}

void UBaseHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	K2_Input_AbilityInputTagReleased(InputTag);
}

void UBaseHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	K2_Input_Move(InputActionValue);
}

void UBaseHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	K2_Input_LookMouse(InputActionValue);
}

void UBaseHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	K2_Input_LookStick(InputActionValue);
}
	
void UBaseHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	K2_Input_Crouch(InputActionValue);
}

void UBaseHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	K2_Input_AutoRun(InputActionValue);
}
