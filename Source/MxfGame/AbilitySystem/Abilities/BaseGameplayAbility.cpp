// Copyright Epic Games, Inc. All Rights Reserved.


#include "BaseGameplayAbility.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseHeroComponent.h"
#include "Player/BasePlayerController.h"

UBaseGameplayAbility::UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = EBaseAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = EBaseAbilityActivationGroup::Independent;
}

UBaseAbilitySystemComponent* UBaseGameplayAbility::GetBaseAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UBaseAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ABasePlayerController* UBaseGameplayAbility::GetBasePlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABasePlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}


AController* UBaseGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ABaseCharacter* UBaseGameplayAbility::GetBaseCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

UBaseHeroComponent* UBaseGameplayAbility::GetHeroComponentFromActorInfo() const
{
	return (CurrentActorInfo ? UBaseHeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}