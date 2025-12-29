// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "MoverCharacter.h"

#include "BaseMoverCharacter.generated.h"

class UCapsuleComponent;
class UCharacterMoverComponent;
class UBaseAbilitySystemComponent;
class ABasePlayerState;
class ABasePlayerController;
class UBaseHealthComponent;
class UBasePawnExtensionComponent;

UCLASS(Config = Game)
class MXFGAME_API ABaseMoverCharacter : public AMoverCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	ABaseMoverCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Base|Pawn")
	ABasePlayerController* GetBasePlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Base|Pawn")
	ABasePlayerState* GetBasePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Base|Pawn")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

	//~APawn interface
	virtual void NotifyControllerChanged() override;
	//~End of APawn interface

protected:

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InitializeGameplayTags();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	// Begins the death sequence for the Pawn (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the Pawn (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UnInitAndDestroy();

	// Called when the death sequence for the Pawn has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base|Pawn", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBasePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base|Pawn", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBaseHealthComponent> HealthComponent;
	
};
