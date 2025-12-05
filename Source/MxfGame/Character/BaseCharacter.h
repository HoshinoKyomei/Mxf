// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
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
class MXFGAME_API ABaseCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABasePlayerController* FindBasePlayerController() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABasePlayerState* FindBasePlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	UBaseAbilitySystemComponent* FindBaseAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// 必须在子类重载
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	UAbilitySystemComponent* FindAbilitySystemComponent() const;
	
};
