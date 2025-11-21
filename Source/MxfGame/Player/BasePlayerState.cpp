// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BasePlayerState.h"

#include "AbilitySystem/Attributes/BaseCombatSet.h"
#include "AbilitySystem/Attributes/BaseHealthSet.h"
#include "AbilitySystem/BaseAbilitySet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/BasePawnData.h"
#include "Character/BasePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "BaseLogChannels.h"
#include "Net/UnrealNetwork.h"

const FName ABasePlayerState::NAME_BaseAbilityReady("BaseAbilitiesReady");

ABasePlayerState::ABasePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UBaseHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UBaseCombatSet>(TEXT("CombatSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

void ABasePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	
	DOREPLIFETIME(ThisClass, StatTags);	
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

void ABasePlayerState::SetPawnData(const UBasePawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogBase, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UBaseAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_BaseAbilityReady);
	
	ForceNetUpdate();
}

void ABasePlayerState::OnRep_PawnData()
{
}

void ABasePlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ABasePlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ABasePlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool ABasePlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}
