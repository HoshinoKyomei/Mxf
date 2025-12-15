// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BasePawnComponent.h"

#include "BaseGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/BaseInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasePawnComponent)

UBasePawnComponent::UBasePawnComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NAME_ActorFeatureName = IGameFrameworkInitStateInterface::GetFeatureName();
	
	GameFrameworkSet.ToStateTags = BaseGameplayTags::InitState_Spawned;
	
	K2_ConstructionScript();
}

UPawnComponent* UBasePawnComponent::FindPawnComponent(const AActor* Actor, const TSubclassOf<UPawnComponent> ComponentClass) const
{
	if (GameFrameworkSet.IsEnabled && Actor)
	{
		if (UPawnComponent* FoundComponent = Cast<UPawnComponent>(Actor->FindComponentByClass(ComponentClass)))
		{
			return FoundComponent;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		FFrame::KismetExecutionMessage(
				TEXT("Can't access pawn component outside of valid pawn context"), 
				ELogVerbosity::Error);
		return nullptr;
	}
}

bool UBasePawnComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                            FGameplayTag DesiredState) const
{
	return K2_CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	K2_HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	K2_OnActorInitStateChanged(Params);
}

void UBasePawnComponent::CheckDefaultInitialization()
{
	K2_CheckDefaultInitialization();
}

bool UBasePawnComponent::K2_HasFeatureReachedInitState(UGameFrameworkComponentManager* Manager, AActor* Actor,
	FName FeatureName, FGameplayTag FeatureState) const
{
	return Manager->HasFeatureReachedInitState(Actor, FeatureName, FeatureState);
}

void UBasePawnComponent::K2_OnRegister_Implementation()
{
	K2_RegisterInitStateFeature();
}

void UBasePawnComponent::OnRegister()
{
	Super::OnRegister();
	
	K2_OnRegister();
}

void UBasePawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GameFrameworkSet.IsEnabled)
	{
		K2_BindOnActorInitStateChanged(GameFrameworkSet);
	}
}

void UBasePawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	if (GameFrameworkSet.IsEnabled)
	{
		UnregisterInitStateFeature();
	}
	
	Super::EndPlay(EndPlayReason);
}

bool UBasePawnComponent::K2_HandleChangeInitState_Implementation(UGameFrameworkComponentManager* Manager,
                                                                            FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
	
	return false;
}

bool UBasePawnComponent::K2_OnActorInitStateChanged_Implementation(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
	return false;
}

bool UBasePawnComponent::K2_CheckDefaultInitialization_Implementation()
{
	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	K2_ContinueInitStateChain();
	
	return true;
}

bool UBasePawnComponent::K2_CanChangeInitState_Implementation(UGameFrameworkComponentManager* Manager,
                                                               FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UBasePawnComponent::K2_RegisterInitStateFeature()
{
	if (!GetPawn<APawn>())
	{
		return;
	}
	
	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UBasePawnComponent::K2_CheckDefaultInitializationForImplementers()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();
}

void UBasePawnComponent::K2_ContinueInitStateChain()
{
	static const TArray<FGameplayTag> StateChain = {
		BaseGameplayTags::InitState_Spawned, BaseGameplayTags::InitState_DataAvailable,
		BaseGameplayTags::InitState_DataInitialized, BaseGameplayTags::InitState_GameplayReady
	};
	
	ContinueInitStateChain(StateChain);
}

void UBasePawnComponent::K2_BindOnActorInitStateChanged(FBaseGameFrameworkSet InGameFrameworkSet)
{
	// Listen for changes to all features
	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(InGameFrameworkSet.FeatureName, InGameFrameworkSet.RequiredStateTags, InGameFrameworkSet.IsCallIfReached);
	
	// Notifies state manager that we have spawned, then try the rest of default initialization
	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(InGameFrameworkSet.ToStateTags));
	CheckDefaultInitialization();
}

bool UBasePawnComponent::K2_HaveAllFeaturesReachedInitState(UGameFrameworkComponentManager* Manager,
	AActor* Actor, FGameplayTag RequiredState, FName ExcludingFeature) const
{
	return Manager->HaveAllFeaturesReachedInitState(Actor, RequiredState, ExcludingFeature);
}
