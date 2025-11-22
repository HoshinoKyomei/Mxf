// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"

#include "BaseCombatSet.generated.h"

class UObject;
struct FFrame;

/**
 * ULyraCombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 */
UCLASS(BlueprintType)
class MXFGAME_API UBaseCombatSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UBaseCombatSet();
	ATTRIBUTE_ACCESSORS(UBaseCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UBaseCombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the healing execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
