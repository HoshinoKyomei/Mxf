// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"

#include "BasePlayerState.generated.h"

class ABasePlayerController;
class UAbilitySystemComponent;
class UBaseAbilitySystemComponent;

/**
 * ABasePlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class MXFGAME_API ABasePlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	ABasePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Base|PlayerState")
	ABasePlayerController* GetBasePlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Base|PlayerState")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//~AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
private:
	
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Base|PlayerState")
	TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;
	
	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UBaseHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UBaseCombatSet> CombatSet;
};
