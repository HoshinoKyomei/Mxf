// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

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
	// Sets default values for this component's properties
	UBaseAbilitySystemComponent();
	
};
