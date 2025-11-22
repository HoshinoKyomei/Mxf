// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BasePawnData.generated.h"

class APawn;
class UBaseAbilitySet;
// class UBaseAbilityTagRelationshipMapping;
// class UBaseInputConfig;
class UObject;

/**
 * UBasePawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Base Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class MXFGAME_API UBasePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UBasePawnData(const FObjectInitializer& ObjectInitializer);

	// Class to instantiate for this pawn (should usually derive from ABasePawn or ABaseCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<UBaseAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	// TObjectPtr<UBaseAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player-controlled pawns to create input mappings and bind input actions.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	// TObjectPtr<UBaseInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	// TSubclassOf<UBaseCameraMode> DefaultCameraMode;
};
