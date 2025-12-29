// Copyright Soatori Games, Inc. All Rights Reserved.

#include "MoverCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "Engine/CollisionProfile.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MoverCharacter)

FName AMoverCharacter::MeshComponentName(TEXT("CharacterMesh0"));
FName AMoverCharacter::CharacterMoverComponentName(TEXT("CharMoverComp"));
FName AMoverCharacter::NavMoverComponentName(TEXT("NavMoverComp"));
FName AMoverCharacter::CapsuleComponentName(TEXT("CollisionCylinder"));

AMoverCharacter::AMoverCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FName ID_MoverCharacters;
		FText NAME_MoverCharacters;
		FConstructorStatics()
			: ID_MoverCharacters(TEXT("MoverCharacters"))
			, NAME_MoverCharacters(NSLOCTEXT("MoverSpriteCategory", "MoverCharacters", "MoverCharacters"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(AMoverCharacter::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;
	
	CharacterMover = CreateDefaultSubobject<UCharacterMoverComponent>(AMoverCharacter::CharacterMoverComponentName);
	
	NavMover = CreateDefaultSubobject<UNavMoverComponent>(AMoverCharacter::NavMoverComponentName);
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(AMoverCharacter::MeshComponentName);
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}
}
