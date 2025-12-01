// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABasePlayerState;
class UBaseAbilitySystemComponent;
class UObject;
class UPlayer;
struct FFrame;

/**
 * ALyraPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class MXFGAME_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	ABasePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ABasePlayerState* GetBasePlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	
	//~AActor interface
	virtual void BeginPlay() override;
	//~End of AActor interface
	
	//~AController interface
	virtual void OnPossess(APawn* InPawn) override;
	//~End of AController interface
	
	//~APlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsAutoRunning() const;
	
protected:
	
	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	void K2_OnEndAutoRun();
};
