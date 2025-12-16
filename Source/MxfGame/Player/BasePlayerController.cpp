// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BasePlayerController.h"

#include "BasePlayerState.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePlayerController)

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ABasePlayerState* ABasePlayerController::GetBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABasePlayerController::GetBaseAbilitySystemComponent() const
{
	const ABasePlayerState* BasePS = GetBasePlayerState();
	return (BasePS ? BasePS->GetBaseAbilitySystemComponent() : nullptr);
}

void ABasePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
