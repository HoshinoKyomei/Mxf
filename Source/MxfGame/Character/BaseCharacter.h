// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "ModularCharacter.h"

#include "BaseCharacter.generated.h"

class AActor;
class AController;
class ABasePlayerController;
class ABasePlayerState;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UInputComponent;
class UAbilitySystemComponent;
class UBaseAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;

/**
 * ABaseCharacter
 *
 *	The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class MXFGAME_API ABaseCharacter : public AModularCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABasePlayerController* FindBasePlayerController() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABasePlayerState* FindBasePlayerState() const;
	
	//~IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Character")
	UBaseAbilitySystemComponent* FindBaseAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	
	// 必须在子类重载
	UFUNCTION(BlueprintImplementableEvent, Category = "Character", meta = (DisplayName = "FindAbilitySystemComponent"))
	UAbilitySystemComponent* K2_FindAbilitySystemComponent() const;
	
	//~IGameplayTagAssetInterface
	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	virtual void GetOwnedGameplayTags(UPARAM(ref) FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	//~End of IGameplayTagAssetInterface
	
protected:
	
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character", meta=(DisplayName = "OnRep_PlayerState"))
	void K2_OnRep_PlayerState();
	UFUNCTION(BlueprintNativeEvent, Category = "Character", meta=(DisplayName = "SetupPlayerInputComponent"))
	void K2_SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character", meta=(DisplayName = "FellOutOfWorld"))
	void K2_FellOutOfWorld(const class UDamageType* dmgType);
	
	virtual bool CanJumpInternal_Implementation() const override;
};