// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BaseLocalPlayer.h"
#include "EnhancedInputSubsystems.h"

UBaseLocalPlayer::UBaseLocalPlayer()
{
}

UEnhancedInputLocalPlayerSubsystem* UBaseLocalPlayer::GetEnhancedInputLocalPlayerSubsystem()
{
	return GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}
