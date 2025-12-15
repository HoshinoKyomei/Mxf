// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBase, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseExperience, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseAbilitySystem, Log, All);
MXFGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogBaseTeams, Log, All);

MXFGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
