// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/BaseHealthComponent.h"
#include "Character/BasePawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "BaseGameplayTags.h"
// #include "BaseLogChannels.h"
// #include "Net/UnrealNetwork.h"
#include "Player/BasePlayerState.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

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

	PawnExtComponent = CreateDefaultSubobject<UBasePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	HealthComponent = CreateDefaultSubobject<UBaseHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

void ABaseCharacter::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UnInitAndDestroy();
}

ABasePlayerState* ABaseCharacter::GetBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABaseCharacter::GetBaseAbilitySystemComponent() const
{
	return Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetBaseAbilitySystemComponent();
}

void ABaseCharacter::OnAbilitySystemInitialized()
{
	UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent();
	check(BaseASC);

	HealthComponent->InitializeWithAbilitySystem(BaseASC);

	InitializeGameplayTags();
}

void ABaseCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ABaseCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void ABaseCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void ABaseCharacter::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		for (const TPair<uint8, FGameplayTag>& TagMapping : BaseGameplayTags::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				BaseASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : BaseGameplayTags::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				BaseASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UBaseCharacterMovementComponent* BaseMoveComp = CastChecked<UBaseCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(BaseMoveComp->MovementMode, BaseMoveComp->CustomMovementMode, true);
	}
}

void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ABaseCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ABaseCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ABaseCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void ABaseCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	HealthComponent->DamageSelfDestruct(/*bFellOutOfWorld=*/ true);
}

void ABaseCharacter::OnDeathStarted(AActor*)
{
	DisableMovementAndCollision();
}

void ABaseCharacter::OnDeathFinished(AActor*)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}


void ABaseCharacter::DisableMovementAndCollision()
{
	if (GetController())
	{
		GetController()->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UBaseCharacterMovementComponent* BaseMoveComp = CastChecked<UBaseCharacterMovementComponent>(GetCharacterMovement());
	BaseMoveComp->StopMovementImmediately();
	BaseMoveComp->DisableMovement();
}

void ABaseCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UnInitAndDestroy();
}


void ABaseCharacter::UnInitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		if (BaseASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void ABaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UBaseCharacterMovementComponent* BaseMoveComp = CastChecked<UBaseCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(BaseMoveComp->MovementMode, BaseMoveComp->CustomMovementMode, true);
}

void ABaseCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = BaseGameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = BaseGameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			BaseASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void ABaseCharacter::ToggleCrouch()
{
	const UBaseCharacterMovementComponent* BaseMoveComp = CastChecked<UBaseCharacterMovementComponent>(GetCharacterMovement());

	if (IsCrouched() || BaseMoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (BaseMoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void ABaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->SetLooseGameplayTagCount(BaseGameplayTags::Status_Crouching, 1);
	}


	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ABaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->SetLooseGameplayTagCount(BaseGameplayTags::Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool ABaseCharacter::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}
