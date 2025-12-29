// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"

#include "MoverCharacter.generated.h"


class UNavMoverComponent;
class UCapsuleComponent;
class UCharacterMoverComponent;

UCLASS(Config = Game, BlueprintType)
class MXFGAME_API AMoverCharacter : public AModularPawn
{
	GENERATED_BODY()

public:

	AMoverCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:
	
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMoverComponent> CharacterMover;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UNavMoverComponent> NavMover;

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
public:
	
	/** Returns Mesh subobject **/
	inline class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName MeshComponentName;
	
	/** Returns CharacterMover subobject **/
	template <class T>
	inline T* GetCharacterMover() const
	{
		return CastChecked<T>(CharacterMover, ECastCheckedType::NullAllowed);
	}
	inline UCharacterMoverComponent* GetCharacterMover() const { return CharacterMover; }
	
	static FName CharacterMoverComponentName;
	
	static FName NavMoverComponentName;
	
	/** Returns CapsuleComponent subobject **/
	inline class UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	/** Name of the CapsuleComponent. */
	static FName CapsuleComponentName;
	
};
