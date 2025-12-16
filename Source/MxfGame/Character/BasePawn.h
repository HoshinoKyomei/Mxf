// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"

#include "BasePawn.generated.h"

UCLASS()
class MXFGAME_API ABasePawn : public AModularPawn
{
	GENERATED_BODY()

public:

	ABasePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
