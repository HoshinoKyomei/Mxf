// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"

#include "BasePlayerController.generated.h"

class ABasePlayerState;
class UBaseAbilitySystemComponent;

/**
 * ABasePlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class MXFGAME_API ABasePlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
public:
	
	ABasePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Base|PlayerController")
	ABasePlayerState* GetBasePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Base|PlayerController")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	
	//~AActor interface
	virtual void BeginPlay() override;
	//~End of AActor interface
	
	//~AController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface
	
	//~Begin of APlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface
	
	UFUNCTION(BlueprintCallable, Category = "Base|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Base|Character")
	bool GetIsAutoRunning() const;
	
protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();
	
private:
	void BroadcastOnPlayerStateChanged();
	
protected:
	
	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	void K2_OnEndAutoRun();
};
