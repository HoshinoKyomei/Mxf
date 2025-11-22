// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BaseVerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FBaseVerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;


UCLASS()
class UBaseVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Base")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Base")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Base")
	static FGameplayCueParameters VerbMessageToCueParameters(const FBaseVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Base")
	static FBaseVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
