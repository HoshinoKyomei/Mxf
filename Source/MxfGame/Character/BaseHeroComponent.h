// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFeatures/BasePawnComponent.h"
#include "BaseHeroComponent.generated.h"

/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 * This depends on a PawnExtensionComponent to coordinate initialization.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class MXFGAME_API UBaseHeroComponent : public UBasePawnComponent
{
	GENERATED_BODY()

public:
	UBaseHeroComponent(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void BindAbilityInput(const APawn* Pawn, const UBaseInputConfig* InputConfig);

	UFUNCTION(BlueprintCallable)
	void SendBindInputsEvent(const APawn* Pawn, const APlayerController* PC);
	
	UFUNCTION(BlueprintCallable)
	void SetupInputBind(UInputComponent* PlayerInputComponent, UEnhancedInputLocalPlayerSubsystem* Subsystem,
	                    const UBaseInputConfig* InputConfig);

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;
	
	UFUNCTION(BlueprintCallable)
	FName GetBindInputsNowName() const { return NAME_BindInputsNow; }
	
protected:
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_AbilityInputTagPressed")
	bool K2_Input_AbilityInputTagPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_AbilityInputTagReleased")
	bool K2_Input_AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_Move")
	bool K2_Input_Move(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_LookMouse")
	bool K2_Input_LookMouse(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_LookStick")
	bool K2_Input_LookStick(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_Crouch")
	bool K2_Input_Crouch(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", DisplayName="Input_AutoRun")
	bool K2_Input_AutoRun(const FInputActionValue& InputActionValue);
};
