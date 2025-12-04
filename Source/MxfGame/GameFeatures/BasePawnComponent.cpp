// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BasePawnComponent.h"

#include "BaseGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePawnComponent)

UBasePawnComponent::UBasePawnComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NAME_ActorFeatureName = IGameFrameworkInitStateInterface::GetFeatureName();
	
	GameFrameworkSet.ToStateTags = BaseGameplayTags::InitState_Spawned;
	
	BaseConstructionScript();
}

bool UBasePawnComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return GameFramework_CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	GameFramework_HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	GameFramework_OnActorInitStateChanged(Params);
}

void UBasePawnComponent::CheckDefaultInitialization()
{
	GameFramework_CheckDefaultInitialization();
}

void UBasePawnComponent::OnRegister()
{
	Super::OnRegister();
	
	if (!GameFramework_OnRegisterInitFeature_Implementation())
	{
		GameFramework_CallRegisterInitStateFeature();
	}
}

void UBasePawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GameFrameworkSet.IsEnabled)
	{
		GameFramework_CallBindOnActorInitStateChanged(NAME_ActorFeatureName, GameFrameworkSet);
	}
}

void UBasePawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

bool UBasePawnComponent::GameFramework_OnRegisterInitFeature_Implementation()
{
	return false;
}

bool UBasePawnComponent::GameFramework_HandleChangeInitState_Implementation(UGameFrameworkComponentManager* Manager,
                                                                            FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
	
	return false;
}

bool UBasePawnComponent::GameFramework_OnActorInitStateChanged_Implementation(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
	return false;
}

bool UBasePawnComponent::GameFramework_CheckDefaultInitialization_Implementation()
{
	GameFramework_CallContinueInitStateChain(TArray<FGameplayTag>());
	
	return true;
}

bool UBasePawnComponent::GameFramework_CanChangeInitState_Implementation(UGameFrameworkComponentManager* Manager,
                                                               FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::GameFramework_CallRegisterInitStateFeature()
{
	if (!GetPawn<APawn>())
	{
		return;
	}
	
	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UBasePawnComponent::GameFramework_CallContinueInitStateChain(TArray<FGameplayTag> InitStateChain)
{
	if (InitStateChain.Num() == 0)
	{
		static const TArray<FGameplayTag> StateChain = {
			BaseGameplayTags::InitState_Spawned, BaseGameplayTags::InitState_DataAvailable,
			BaseGameplayTags::InitState_DataInitialized, BaseGameplayTags::InitState_GameplayReady
		};

		InitStateChain = StateChain;
	}
	
	ContinueInitStateChain(InitStateChain);
}

void UBasePawnComponent::GameFramework_CallBindOnActorInitStateChanged(FName FeatureName, FBaseGameFrameworkSet InGameFrameworkSet)
{
	// Listen for changes to all features
	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(FeatureName, InGameFrameworkSet.RequiredStateTags, InGameFrameworkSet.IsCallIfReached);
	
	// Notifies state manager that we have spawned, then try the rest of default initialization
	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(InGameFrameworkSet.ToStateTags));
	CheckDefaultInitialization();
}