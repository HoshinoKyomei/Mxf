// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "BaseGameplayAbility.generated.h"

class UBaseAbilitySystemComponent;

/**
 * UBaseGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class MXFGAME_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	friend class UBaseAbilitySystemComponent;
	
public:
	
	UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
