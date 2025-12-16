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
	
	//~Begin of APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface
};
