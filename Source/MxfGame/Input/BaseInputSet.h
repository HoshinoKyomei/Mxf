// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BaseInputSet.generated.h"

class UBaseInputConfig;
class UBaseInputComponent;

USTRUCT(BlueprintType)
struct FBaseInputSet_TriggerFunc
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	ETriggerEvent TriggerEvent;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FName FuncName;
};

/**
 * FBaseInputSet_InputMapping
 */
USTRUCT(BlueprintType)
struct FBaseInputSet_InputMapping
{
	GENERATED_BODY()

public:

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TSubclassOf<UObject> FuncClass;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FBaseInputSet_TriggerFunc> TriggeredFunc;
};

/**
 * 
 */
UCLASS()
class MXFGAME_API UBaseInputSet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UBaseInputSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void FindInputMappingForTag(const FGameplayTag& InputTag, TArray<FBaseInputSet_InputMapping>& OutInputMapping);
	
	/** Input mappings to bind */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(TitleProperty="InputTag"))
	TArray<FBaseInputSet_InputMapping> InputMappings;
	
};
