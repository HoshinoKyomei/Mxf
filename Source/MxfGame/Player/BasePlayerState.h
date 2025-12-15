// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"

#include "BasePlayerState.generated.h"

/**
 * ABasePlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class MXFGAME_API ABasePlayerState : public AModularPlayerState
{
	GENERATED_BODY()
	
public:
	
	ABasePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
