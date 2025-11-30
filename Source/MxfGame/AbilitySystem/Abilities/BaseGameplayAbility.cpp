// Copyright Epic Games, Inc. All Rights Reserved.


#include "BaseGameplayAbility.h"

UBaseGameplayAbility::UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
}

UBaseAbilitySystemComponent* UBaseGameplayAbility::GetBaseAbilitySystemComponentFromActorInfo() const
{
	return nullptr;
}

ABasePlayerController* UBaseGameplayAbility::GetBasePlayerControllerFromActorInfo() const
{
	return nullptr;
}

AController* UBaseGameplayAbility::GetControllerFromActorInfo() const
{
	return nullptr;
}

ABaseCharacter* UBaseGameplayAbility::GetBaseCharacterFromActorInfo() const
{
	return nullptr;
}

UBaseHeroComponent* UBaseGameplayAbility::GetHeroComponentFromActorInfo() const
{
	return nullptr;
}
