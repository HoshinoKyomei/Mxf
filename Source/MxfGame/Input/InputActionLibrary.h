// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "InputActionLibrary.generated.h"

/**
 * InputActionLibrary
 */
UCLASS()
class MXFGAME_API UInputActionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	// 获取布尔值 (Digital)
	UFUNCTION(BlueprintPure, Category = "Input")
	static bool GetInputActionValueToBool(const FInputActionValue& Value) { return Value.Get<bool>(); }

	// 获取浮点值 (Axis1D)
	UFUNCTION(BlueprintPure, Category = "Input")
	static float GetInputActionValueToFloat(const FInputActionValue& Value) { return Value.Get<float>(); }

	// 获取二维向量 (Axis2D) - 对应 IA_Look
	UFUNCTION(BlueprintPure, Category = "Input")
	static FVector2D GetInputActionValueToVector2D(const FInputActionValue& Value) { return Value.Get<FVector2D>(); }

	// 获取三维向量 (Axis3D)
	UFUNCTION(BlueprintPure, Category = "Input")
	static FVector GetInputActionValueToVector(const FInputActionValue& Value) { return Value.Get<FVector>(); }
	
	UFUNCTION(BlueprintPure, Category = "Input", meta=(NativeBreakFunc, BlueprintThreadSafe))
	static void BreakInputActionValue(const FInputActionValue& Value, bool& bBool, float& Float, FVector2D& Vector2D, FVector& Vector)
	{
		bBool = Value.Get<bool>();
		Float = Value.Get<float>();
		Vector2D = Value.Get<FVector2D>();
		Vector = Value.Get<FVector>();
	}
	
};
