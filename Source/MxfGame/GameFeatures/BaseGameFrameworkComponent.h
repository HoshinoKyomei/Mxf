// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"

#include "BaseGameFrameworkComponent.generated.h"


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class MXFGAME_API UBaseGameFrameworkComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	
	UBaseGameFrameworkComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	bool BaseConstructionScript();
	
};
