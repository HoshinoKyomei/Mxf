// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseGameFrameworkComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseGameFrameworkComponent)

UBaseGameFrameworkComponent::UBaseGameFrameworkComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	K2_ConstructionScript();
}

void UBaseGameFrameworkComponent::OnUnregister()
{
	K2_OnUnregister();
	
	Super::OnUnregister();
}
