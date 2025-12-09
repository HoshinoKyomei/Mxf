// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"

#include "BaseAttributeSet.generated.h"

class AActor;
class UBaseAbilitySystemComponent;
class UObject;
class UWorld;
struct FGameplayEffectSpec;

struct FGameplayEffectModCallbackData;

/** Data that describes what happened in an attribute modification. This is passed to ability set callbacks */
USTRUCT(BlueprintType)
struct FBaseGameplayModifierEvaluatedData
{
	GENERATED_USTRUCT_BODY()

	FBaseGameplayModifierEvaluatedData()
		: Attribute()
		, ModifierOp(EGameplayModOp::Additive)
		, Magnitude(0.f)
		, IsValid(false)
	{
	}
	
	FBaseGameplayModifierEvaluatedData(const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> InModOp, float InMagnitude, FActiveGameplayEffectHandle InHandle = FActiveGameplayEffectHandle())
		: Attribute(InAttribute)
		, ModifierOp(InModOp)
		, Magnitude(InMagnitude)
		, Handle(InHandle)
		, IsValid(true)
	{
	}

	/** What attribute was modified */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute Attribute;

	/** The numeric operation of this modifier: Override, Add, Multiply, etc  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp;

	/** The raw magnitude of the applied attribute, this is generally before being clamped */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Magnitude;

	/** Handle of the active gameplay effect that originated us. Will be invalid in many cases */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FActiveGameplayEffectHandle	Handle;

	/** True if something was evaluated */
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	bool IsValid;

	FString ToSimpleString() const
	{
		return FString::Printf(TEXT("%s %s EvalMag: %f"), *Attribute.GetName(), *EGameplayModOpToString(ModifierOp), Magnitude);
	}
};


/**
 * This macro defines a set of helper functions for accessing and initializing attributes.
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(UBaseHealthSet, Health)
 * will create the following functions:
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** 
 * Delegate used to broadcast attribute events, some of these parameters may be null on clients: 
 * @param EffectInstigator	The original instigating actor for this event
 * @param EffectCauser		The physical actor that caused the change
 * @param EffectSpec		The full effect spec for this change
 * @param EffectMagnitude	The raw magnitude, this is before clamping
 * @param OldValue			The value of the attribute before it was changed
 * @param NewValue			The value after it was changed
*/
DECLARE_MULTICAST_DELEGATE_SixParams(FBaseAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

/**
 * UBaseAttributeSet
 *
 *	Base attribute set class for the project.
 */
UCLASS()
class MXFGAME_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UBaseAttributeSet();

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable)
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ConstructionScript"))
	bool K2_ConstructionScript();
	
	
	//~UAttributeSet interface
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~End of UAttributeSet interface
	
	/**
	 * Blueprint-implementable event for PreGameplayEffectExecute.
	 * This is called before an attribute is changed. It can be used to clamp the change, or to cancel it entirely.
	 *
	 * @param InEffectSpec		The spec of the gameplay effect that is being applied.
	 * @param InEvaluatedData	The data about the attribute modification. This can be modified to change the outcome.
	 * @return					Return true to continue with the attribute change, or false to cancel it.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Attributes", meta=(DisplayName="PreGameplayEffectExecute"))
	bool K2_PreGameplayEffectExecute(const FGameplayEffectSpec& InEffectSpec, UPARAM(ref) FBaseGameplayModifierEvaluatedData& InEvaluatedData, UAbilitySystemComponent* InTarget);

	/**
	 * Blueprint-implementable event for PostGameplayEffectExecute.
	 * This is called after an attribute has been changed.
	 *
	 * @param InEffectSpec		The spec of the gameplay effect that was applied.
	 * @param InEvaluatedData	The data about the attribute modification.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes", meta = (DisplayName = "PostGameplayEffectExecute"))
	bool K2_PostGameplayEffectExecute(const FGameplayEffectSpec& InEffectSpec, const FBaseGameplayModifierEvaluatedData& InEvaluatedData, UAbilitySystemComponent* InTarget);

	/**
	 * Blueprint-implementable event for PreAttributeBaseChange.
	 * This is called before an attribute's base value is changed. It can be used to clamp the new value.
	 *
	 * @param Attribute			The attribute that is about to be changed.
	 * @param NewBaseValue		The new base value of the attribute. This can be modified to clamp the value.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Attributes", meta = (DisplayName = "PreAttributeBaseChange"))
	bool K2_PreAttributeBaseChange(const FGameplayAttribute& Attribute, UPARAM(ref) float& NewBaseValue) const;

	/**
	 * Blueprint-implementable event for PostAttributeBaseChange.
	 * This is called after an attribute's base value has been changed.
	 *
	 * @param Attribute			The attribute that was changed.
	 * @param OldBaseValue		The value of the attribute's base before the change.
	 * @param NewBaseValue		The value of the attribute's base after the change.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes", meta = (DisplayName = "PostAttributeBaseChange"))
	bool K2_PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldBaseValue, float NewBaseValue) const;

	/**
	 * Blueprint-implementable event for PreAttributeChange.
	 * This is called before any change happens to an attribute's final value. This is the best place to clamp the final value of an attribute.
	 *
	 * @param Attribute			The attribute that is about to be changed.
	 * @param NewValue			The new final value of the attribute. This can be modified to clamp the value.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Attributes", meta = (DisplayName = "PreAttributeChange"))
	bool K2_PreAttributeChange(const FGameplayAttribute& Attribute, UPARAM(ref) float& NewValue);

	/**
	 * Blueprint-implementable event for PostAttributeChange.
	 * This is called after any change happens to an attribute's final value.
	 *
	 * @param Attribute			The attribute that was changed.
	 * @param OldValue			The value of the attribute before the change.
	 * @param NewValue			The value of the attribute after the change.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes", meta = (DisplayName = "PostAttributeChange"))
	bool K2_PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue);
	
protected:
	
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "GetDynamicAssetTags"))
	FGameplayTagContainer K2_GetDynamicAssetTags(const FGameplayEffectSpec& InEffectSpec) const;
};
