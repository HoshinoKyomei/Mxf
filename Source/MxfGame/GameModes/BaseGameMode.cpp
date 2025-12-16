// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseGameMode.h"

#include "Character/BaseCharacter.h"
#include "Player/BasePlayerController.h"
#include "Player/BasePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseGameMode)

ABaseGameMode::ABaseGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// GameStateClass = ABaseGameState::StaticClass();
	// GameSessionClass = ABaseGameSession::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
	// ReplaySpectatorPlayerControllerClass = ABaseReplayPlayerController::StaticClass();
	PlayerStateClass = ABasePlayerState::StaticClass();
	DefaultPawnClass = ABaseCharacter::StaticClass();
	// HUDClass = ABaseHUD::StaticClass();
}
