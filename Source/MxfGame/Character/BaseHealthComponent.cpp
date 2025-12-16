// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/BaseHealthComponent.h"

#include "AbilitySystem/Attributes/BaseAttributeSet.h"
#include "BaseLogChannels.h"
// #include "System/BaseAssetManager.h"
// #include "System/BaseGameData.h"
#include "BaseGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/BaseHealthSet.h"
// #include "Messages/BaseVerbMessage.h"
// #include "Messages/BaseVerbMessageHelpers.h"
// #include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseHealthComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Base_Elimination_Message, "Base.Elimination.Message");


UBaseHealthComponent::UBaseHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	DeathState = EBaseDeathState::NotDead;
}

void UBaseHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseHealthComponent, DeathState);
}

void UBaseHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();

	Super::OnUnregister();
}

void UBaseHealthComponent::InitializeWithAbilitySystem(UBaseAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UBaseHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register to listen for attribute changes.
	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(UBaseHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());

	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
}

void UBaseHealthComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();

	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

void UBaseHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(BaseGameplayTags::Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(BaseGameplayTags::Status_Death_Dead, 0);
	}
}

float UBaseHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UBaseHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UBaseHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

void UBaseHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UBaseHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UBaseHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = BaseGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec->Def;
			Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

		// Send a standardized verb message that other systems can observe
		// {
		// 	FBaseVerbMessage Message;
		// 	Message.Verb = TAG_Base_Elimination_Message;
		// 	Message.Instigator = DamageInstigator;
		// 	Message.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
		// 	Message.Target = UBaseVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
		// 	Message.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
		// 	//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
		// 	//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...
		//
		// 	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		// 	MessageSystem.BroadcastMessage(Message.Verb, Message);
		// }

		//@TODO: assist messages (could compute from damage dealt elsewhere)?
	}

#endif // #if WITH_SERVER_CODE
}

void UBaseHealthComponent::OnRep_DeathState(EBaseDeathState OldDeathState)
{
	const EBaseDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogBase, Warning, TEXT("BaseHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDeathState == EBaseDeathState::NotDead)
	{
		if (NewDeathState == EBaseDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EBaseDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDeathState == EBaseDeathState::DeathStarted)
	{
		if (NewDeathState == EBaseDeathState::DeathFinished)
		{
			FinishDeath();
		}
		else
		{
			UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DeathState == NewDeathState), TEXT("BaseHealthComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
}

void UBaseHealthComponent::StartDeath()
{
	if (DeathState != EBaseDeathState::NotDead)
	{
		return;
	}

	DeathState = EBaseDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(BaseGameplayTags::Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UBaseHealthComponent::FinishDeath()
{
	if (DeathState != EBaseDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EBaseDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(BaseGameplayTags::Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UBaseHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if ((DeathState == EBaseDeathState::NotDead) && AbilitySystemComponent)
	{
		// const TSubclassOf<UGameplayEffect> DamageGE = UBaseAssetManager::GetSubclass(UBaseGameData::Get().DamageGameplayEffect_SetByCaller);
		// if (!DamageGE)
		// {
		// 	UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to find gameplay effect [%s]."), *GetNameSafe(GetOwner()), *UBaseGameData::Get().DamageGameplayEffect_SetByCaller.GetAssetName());
		// 	return;
		// }

		// FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
		// FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		// if (!Spec)
		// {
		// 	UE_LOG(LogBase, Error, TEXT("BaseHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to make outgoing spec for [%s]."), *GetNameSafe(GetOwner()), *GetNameSafe(DamageGE));
		// 	return;
		// }
		
		// Spec->AddDynamicAssetTag(TAG_Gameplay_DamageSelfDestruct);
		
		// if (bFellOutOfWorld)
		// {
		// 	Spec->AddDynamicAssetTag(TAG_Gameplay_FellOutOfWorld);
		// }
		
		// const float DamageAmount = GetMaxHealth();
		
		// Spec->SetSetByCallerMagnitude(BaseGameplayTags::SetByCaller_Damage, DamageAmount);
		// AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

