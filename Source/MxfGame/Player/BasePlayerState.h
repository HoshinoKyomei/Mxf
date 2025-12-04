// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"

#include "BasePlayerState.generated.h"

class AController;
class ABasePlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UBaseAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayTag;

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
	
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerState")
	ABasePlayerController* GetBasePlayerController() const;

	UFUNCTION(BlueprintNativeEvent, Category = "PlayerState")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent_Implementation() const { return AbilitySystemComponent; }
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//~AActor interface
	// virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
	static const FName NAME_BaseAbilityReady;
	
protected:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;
	
	// Health attribute set used by this actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerState")
	TObjectPtr<const class UBaseHealthSet> HealthSet;
};
