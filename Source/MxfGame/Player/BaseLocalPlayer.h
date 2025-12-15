// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "BaseLocalPlayer.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

/**
 * 
 */
UCLASS(Blueprintable)
class MXFGAME_API UBaseLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:
	
	UBaseLocalPlayer();
	
	UFUNCTION(BlueprintPure, Category = "PlayerController")
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();
};
