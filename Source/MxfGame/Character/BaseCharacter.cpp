// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseGameplayTags.h"
#include "BaseLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "Player/BasePlayerController.h"
#include "Player/BasePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

class AActor;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UInputComponent;

static FName NAME_BaseCharacterCollisionProfile_Capsule(TEXT("BasePawnCapsule"));
static FName NAME_BaseCharacterCollisionProfile_Mesh(TEXT("BasePawnMesh"));

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Mesh);

	UBaseCharacterMovementComponent* BaseMoveComp = CastChecked<UBaseCharacterMovementComponent>(GetCharacterMovement());
	BaseMoveComp->GravityScale = 1.0f;
	BaseMoveComp->MaxAcceleration = 2400.0f;
	BaseMoveComp->BrakingFrictionFactor = 1.0f;
	BaseMoveComp->BrakingFriction = 6.0f;
	BaseMoveComp->GroundFriction = 8.0f;
	BaseMoveComp->BrakingDecelerationWalking = 1400.0f;
	BaseMoveComp->bUseControllerDesiredRotation = false;
	BaseMoveComp->bOrientRotationToMovement = false;
	BaseMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	BaseMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	BaseMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	BaseMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	BaseMoveComp->SetCrouchedHalfHeight(65.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

ABasePlayerController* ABaseCharacter::FindBasePlayerController() const
{
	return Cast<ABasePlayerController>(GetController());
}

ABasePlayerState* ABaseCharacter::FindBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABaseCharacter::FindBaseAbilitySystemComponent() const
{
	return Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return FindAbilitySystemComponent();
}