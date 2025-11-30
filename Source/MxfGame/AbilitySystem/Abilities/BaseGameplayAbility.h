// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

class ABasePlayerController;
class ABaseCharacter;
class UBaseHeroComponent;
class UObject;

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class MXFGAME_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	friend class UBaseAbilitySystemComponent;
	 
public:
	
	UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Ability")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	ABasePlayerController* GetBasePlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	ABaseCharacter* GetBaseCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	UBaseHeroComponent* GetHeroComponentFromActorInfo() const;
};
