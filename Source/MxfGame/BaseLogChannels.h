// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogMacros.h"
#include "BaseLogChannels.generated.h"

class UObject;

MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBase, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseExperience, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseAbilitySystem, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseTeams, Log, All);

MXFGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);

UENUM(BlueprintType)
enum class EBaseLogVerbosity : uint8
{
	Log,
	Warning,
	Error
};

/**
 * A library of global logging functions for Blueprint.
 */
UCLASS()
class MXFGAME_API UBaseLogChannels : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Prints a message to the 'LogBase' log category with a specified verbosity level.
	 * This can be called from any Blueprint.
	 * @param Message The message to log.
	 * @param Verbosity The logging level (Log, Warning, or Error).
	 */
	UFUNCTION(BlueprintCallable, Category = "Logging", meta=(DevelopmentOnly, DisplayName="Log Message (Base)"))
	static void LogChannels(const FString& Message, EBaseLogVerbosity Verbosity = EBaseLogVerbosity::Log);
};
