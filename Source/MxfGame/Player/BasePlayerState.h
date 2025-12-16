// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "System/GameplayTagStack.h"

#include "BasePlayerState.generated.h"

class UBasePawnData;
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
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UBasePawnData* InPawnData);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface
	
	static const FName NAME_BaseAbilityReady;
	
	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	bool HasStatTag(FGameplayTag Tag) const;
	
protected:
	UFUNCTION()
	void OnRep_PawnData();

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UBasePawnData> PawnData;
	
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
	
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
};
