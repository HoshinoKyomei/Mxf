// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseLogChannels.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogBase);
DEFINE_LOG_CATEGORY(LogBaseExperience);
DEFINE_LOG_CATEGORY(LogBaseAbilitySystem);
DEFINE_LOG_CATEGORY(LogBaseTeams);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}

void UBaseLogChannels::LogChannels(const FString& Message, EBaseLogVerbosity Verbosity)
{
	// This will now print the message to the specified log category.
	UE_LOG(LogBase, Log, TEXT("%s"), *Message);
	switch (Verbosity)
	{
	case EBaseLogVerbosity::Warning:
		UE_LOG(LogBase, Warning, TEXT("%s"), *Message);
		break;
	case EBaseLogVerbosity::Error:
		UE_LOG(LogBase, Error, TEXT("%s"), *Message);
		break;
	case EBaseLogVerbosity::Log:
	default:
		UE_LOG(LogBase, Log, TEXT("%s"), *Message);
		break;
	}
}

