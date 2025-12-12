// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "BaseAbilitySystemComponent.generated.h"

class AActor;
class UGameplayAbility;
class UObject;
struct FFrame;

/**
 * UBaseAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS(Blueprintable)
class MXFGAME_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UBaseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	
	//~ AbilitySystemComponent Interface
	/** Returns the avatar actor for this component */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="GetAvatarActor"))
	AActor* K2_GetBaseAvatarActor() const { return GetAvatarActor(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="GetAbilityActorInfo"))
	AActor* K2_GetAbilityActorInfo() const { return AbilityActorInfo->OwnerActor.Get(); }
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="GetOwnerActor"))
	AActor* K2_GetOwnerActor() const { return GetOwnerActor(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="RefreshAbilityActorInfo"))
	void K2_RefreshAbilityActorInfo() { RefreshAbilityActorInfo(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="InitAbilityActorInfo"))
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="SetNumericAttributeBase"))
	void K2_SetNumericAttributeBase(const FGameplayAttribute &Attribute, float NewBaseValue);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="SetLooseGameplayTagCount"))
	void K2_SetLooseGameplayTagCount(const FGameplayTag& GameplayTag, int32 NewCount);
	
	//~ End AbilitySystemComponent Interface
	
public:
	
};
