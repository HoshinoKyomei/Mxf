// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

class UObject;

/**
 * UBaseAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MXFGAME_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UBaseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
