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
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	ABasePlayerController* GetBasePlayerController() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	ABasePlayerState* GetBasePlayerState() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	UAbilitySystemComponent* FindAbilitySystemComponent() const;
	
};
