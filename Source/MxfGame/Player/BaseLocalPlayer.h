// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"

#include "BaseLocalPlayer.generated.h"

/**
 * UBaseLocalPlayer
 */
UCLASS(Blueprintable, config=Engine, transient)
class MXFGAME_API UBaseLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
	
public:
	
	UBaseLocalPlayer();
	
};
