// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "BaseCharacter.generated.h"

/**
 * ABaseCharacter
 *
 *	The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class MXFGAME_API ABaseCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
