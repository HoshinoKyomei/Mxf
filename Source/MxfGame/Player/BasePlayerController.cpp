// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BasePlayerController.h"

#include "BasePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePlayerController)

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ABasePlayerState* ABasePlayerController::GetBasePlayerState_Implementation() const
{
	return CastChecked<ABasePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABasePlayerController::GetBaseAbilitySystemComponent_Implementation() const
{
	const ABasePlayerState* BasePS = GetBasePlayerState();
	return (BasePS ? BasePS->GetBaseAbilitySystemComponent() : nullptr);
}
