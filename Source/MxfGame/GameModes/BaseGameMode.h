// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "BaseGameMode.generated.h"

/**
 * ABaseGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class MXFGAME_API ABaseGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
	
public:
	
	ABaseGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
