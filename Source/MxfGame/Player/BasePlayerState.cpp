// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BasePlayerState.h"

#include "BasePlayerController.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePlayerState)

ABasePlayerState::ABasePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UBaseAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.

	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

ABasePlayerController* ABasePlayerState::GetBasePlayerController() const
{
	return Cast<ABasePlayerController>(GetOwner());
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
}

void ABasePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}
