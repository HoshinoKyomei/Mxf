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
	
	//~ 蓝图父类方法 Interface
	/** Returns the avatar actor for this component */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="GetAvatarActor"))
	AActor* GetBaseAvatarActor() const { return GetAvatarActor(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent")
	AActor* GetAbilityActorInfo() const { return AbilityActorInfo->OwnerActor.Get(); }
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="GetOwnerActor"))
	AActor* GetBaseOwnerActor() const { return GetOwnerActor(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="RefreshAbilityActorInfo"))
	void BaseRefreshAbilityActorInfo() { RefreshAbilityActorInfo(); }
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent", meta=(DisplayName="InitAbilityActorInfo"))
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	//~ End 蓝图父类方法 Interface
	
public:
	
};
