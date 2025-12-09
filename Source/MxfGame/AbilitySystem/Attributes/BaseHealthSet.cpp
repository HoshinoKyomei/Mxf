// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseHealthSet.h"
#include "AbilitySystem/Attributes/BaseAttributeSet.h"
#include "BaseGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Engine/World.h"
#include "GameplayEffectExtension.h"
// #include "Messages/BaseVerbMessage.h"
// #include "GameFramework/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseHealthSet)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageSelfDestruct, "Gameplay.Damage.SelfDestruct");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_FellOutOfWorld, "Gameplay.Damage.FellOutOfWorld");
UE_DEFINE_GAMEPLAY_TAG(TAG_Base_Damage_Message, "Base.Damage.Message");

UBaseHealthSet::UBaseHealthSet()
	// : Health(100.0f)
	// , MaxHealth(100.0f)
{
	// bOutOfHealth = false;
	// MaxHealthBeforeAttributeChange = 0.0f;
	// HealthBeforeAttributeChange = 0.0f;
}

void UBaseHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UBaseHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseHealthSet, Health, OldValue);
	
	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	// These events on the client should not be changing attributes
	
	K2_OnRep_Health(OldValue);
}

void UBaseHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseHealthSet, MaxHealth, OldValue);
	
	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	
	K2_OnRep_MaxHealth(OldValue);
}
