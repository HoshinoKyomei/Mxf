// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class MXFGAME_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	
	UBaseCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	
};
