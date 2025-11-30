// Copyright Epic Games, Inc. All Rights Reserved.


#include "BasePlayerController.h"

#include "BaseGameplayTags.h"
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

void ABasePlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool ABasePlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		bIsAutoRunning = BaseASC->GetTagCount(BaseGameplayTags::Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
}

void ABasePlayerController::OnStartAutoRun()
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->SetLooseGameplayTagCount(BaseGameplayTags::Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}	
}

void ABasePlayerController::OnEndAutoRun()
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->SetLooseGameplayTagCount(BaseGameplayTags::Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
}
