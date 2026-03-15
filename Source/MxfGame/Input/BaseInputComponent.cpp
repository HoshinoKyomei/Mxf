// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseInputComponent.h"

#include "EnhancedInputSubsystems.h"
// #include "Player/BaseLocalPlayer.h"
// #include "Settings/BaseSettingsLocal.h"
#include "BaseInputSet.h"
#include "BaseLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseInputComponent)

class UBaseInputConfig;

UBaseInputComponent::UBaseInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UBaseInputComponent::AddInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UBaseInputComponent::RemoveInputMappings(const UBaseInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UBaseInputComponent::BindFunctionActionForTag(const UBaseInputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UObject* Object, FName Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindFunctionInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

void UBaseInputComponent::BindFunctionActions(const UBaseInputConfig* InputConfig, UObject* Object,
	const UBaseInputSet* InputSet)
{
	check(InputConfig);
	check(InputSet);
	if (!Object)
	{
		return;
	}
	
	
    // 缓存已解析的对象引用。Key 是类，Value 是该类在 Pawn 身上对应的实例。
    // 使用 TMap 优化，避免多次调用 GetComponentByClass 这个高开销操作
    TMap<TSubclassOf<UObject>, UObject*> CacheObjects;

    // 遍历 InputSet 中策划配置的所有映射行
    for (const FBaseInputSet_InputMapping& InputMapping : InputSet->InputMappings)
    {
    	// 只有 Tag 有效且指定了目标类，才进行处理
    	if (!InputMapping.InputTag.IsValid() || InputMapping.FuncClass == nullptr)
    	{
    		continue;
    	}

    	UObject* TargetObject = nullptr;
    	// --- 核心逻辑：获取执行对象实例 ---
    	// 使用二级指针接收 Find 的结果
    	if (UObject** CachedObj = CacheObjects.Find(InputMapping.FuncClass))
    	{
    		// 情况 A：找到了缓存。直接解引用拿到之前存好的对象地址。
    		TargetObject = *CachedObj;
    	}
    	else
    	{
    		// 情况 B：首次查找。
    		// 按照你的设计逻辑：先看 Pawn 自身是不是这个类，不是的话再去搜组件。
    		if (Object->IsA(InputMapping.FuncClass))
    		{
    			TargetObject = Object;
    		}
    		else
    		{
    			AActor* Actor = Cast<AActor>(Object);
    			if (!Actor)
    			{
    				UE_LOG(LogBase, Error, TEXT("BindFunctionActions: Object is not an AActor"));
    				continue;
    			}

    			if (!InputMapping.FuncClass->IsChildOf<UActorComponent>())
    			{
    				UE_LOG(LogBase, Error, TEXT("BindFunctionActions: FuncClass %s is not a UActorComponent"),
						*InputMapping.FuncClass->GetName());
    				continue;
    			}

    			TargetObject = Actor->GetComponentByClass(*InputMapping.FuncClass);
    			// TargetObject = CastChecked<AActor>(Object)->GetComponentByClass(*InputMapping.FuncClass);
    		}

    		// 将结果存入缓存。注意：即便 TargetObject 是 nullptr 也要存，
    		// 这样下次遇到同一个类就知道“搜过了，没找到”，不用再搜一次。
    		CacheObjects.Add(InputMapping.FuncClass, TargetObject);
    	}
    	// 如果最终没找到对象引用，跳过这一行的绑定并打印警告（可选）
    	if (!TargetObject)
    	{
    		UE_LOG(LogBase, Warning, TEXT("BindFunctionActions: 无法为 Tag [%s] 找到类 [%s] 的实例 (Actor: %s)"), 
				*InputMapping.InputTag.ToString(), *InputMapping.FuncClass->GetName(), *Object->GetName());
    		continue;
    	}

    	// --- 核心逻辑：执行真正绑定 ---
    	// 1. 从 InputConfig 的 FunctionInputActions 中查找对应的 IA 资源
    	const UInputAction* IA = InputConfig->FindFunctionInputActionForTag(InputMapping.InputTag);
    	if (!IA) 
    	{
    		UE_LOG(LogBase, Warning, TEXT("找不到InputAction for Tag [%s]"), *InputMapping.InputTag.ToString());
    		continue;
    	}

    	// 2. 遍历该映射行中配置的所有触发事件（如 Started, Triggered）
    	for (const FBaseInputSet_TriggerFunc& Trigger : InputMapping.TriggeredFunc)
    	{
    		if (Trigger.FuncName.IsNone())
    		{
    			continue;
    		}
    		// 使用增强输入的 FName 绑定接口进行动态绑定
    		// TargetObject 可以是 Pawn 自身，也可以是它身上的任何 ActorComponent
    		BindAction(IA, Trigger.TriggerEvent, TargetObject, Trigger.FuncName);
    	}
    }
}


void UBaseInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
