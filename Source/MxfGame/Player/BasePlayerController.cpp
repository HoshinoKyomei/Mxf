// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BasePlayerController.h"

#include "BasePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePlayerController)

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ABasePlayerState* ABasePlayerController::FindBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABasePlayerController::FindBaseAbilitySystemComponent() const
{
	const ABasePlayerState* BasePS = FindBasePlayerState();
	return (BasePS ? BasePS->FindBaseAbilitySystemComponent() : nullptr);
}
