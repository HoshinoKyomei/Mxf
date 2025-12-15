// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"

#include "BasePlayerController.generated.h"

class UBaseLocalPlayer;
class UEnhancedInputLocalPlayerSubsystem;
class ABasePlayerState;
class APawn;
class APlayerState;
class FPrimitiveComponentId;
class IInputInterface;
class UBaseAbilitySystemComponent;
class UObject;
class UPlayer;
struct FFrame;

/**
 * ABasePlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class MXFGAME_API ABasePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
public:
	
	ABasePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ABasePlayerState* FindBasePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	UBaseAbilitySystemComponent* FindBaseAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintPure, Category = "PlayerController")
	UBaseLocalPlayer* GetBaseLocalPlayer() const;
	
};