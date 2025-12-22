// Copyright Soatori Games, Inc. All Rights Reserved.


#include "BaseInputSet.h"

#include "BaseInputComponent.h"
#include "BaseInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseInputSet)

UBaseInputSet::UBaseInputSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBaseInputSet::FindInputMappingForTag(const FGameplayTag& InputTag, TArray<FBaseInputSet_InputMapping>& OutInputMapping)
{
	for (FBaseInputSet_InputMapping& Mapping : InputMappings)
	{
		if (InputTag == Mapping.InputTag)
		{
			OutInputMapping.Add(Mapping);
		}
	}
}
