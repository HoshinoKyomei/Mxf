// Copyright Soatori Games, Inc. All Rights Reserved.

#include "BaseMoverCharacter.h"

#include "BaseGameplayTags.h"
#include "BaseHealthComponent.h"
#include "BasePawnExtensionComponent.h"
#include "TimerManager.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Player/BasePlayerController.h"
#include "Player/BasePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseMoverCharacter)

static FName NAME_BaseMoverCharacterCollisionProfile_Capsule(TEXT("BaseMoverCharacterCapsule"));
static FName NAME_BaseMoverCharacterCollisionProfile_Mesh(TEXT("BaseMoverCharacterMesh"));

ABaseMoverCharacter::ABaseMoverCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	// Avoid ticking ABaseMoverCharacters if possible.
	// PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.bStartWithTickEnabled = true;
	
	SetNetCullDistanceSquared(900000000.0f);
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_BaseMoverCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_BaseMoverCharacterCollisionProfile_Mesh);

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
	
}


void ABaseMoverCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ABaseMoverCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseMoverCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseMoverCharacter::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UnInitAndDestroy();
}

void ABaseMoverCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseMoverCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

ABasePlayerController* ABaseMoverCharacter::GetBasePlayerController() const
{
	return Cast<ABasePlayerController>(GetController());
}

ABasePlayerState* ABaseMoverCharacter::GetBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UBaseAbilitySystemComponent* ABaseMoverCharacter::GetBaseAbilitySystemComponent() const
{
	return Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ABaseMoverCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetBaseAbilitySystemComponent();
}

void ABaseMoverCharacter::OnAbilitySystemInitialized()
{
	UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent();
	check(BaseASC);

	HealthComponent->InitializeWithAbilitySystem(BaseASC);

	InitializeGameplayTags();
}

void ABaseMoverCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
}

void ABaseMoverCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ABaseMoverCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void ABaseMoverCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ABaseMoverCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void ABaseMoverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void ABaseMoverCharacter::InitializeGameplayTags()
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
	}
}

void ABaseMoverCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		BaseASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ABaseMoverCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ABaseMoverCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ABaseMoverCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UBaseAbilitySystemComponent* BaseASC = GetBaseAbilitySystemComponent())
	{
		return BaseASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void ABaseMoverCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	HealthComponent->DamageSelfDestruct(/*bFellOutOfWorld=*/ true);
}

void ABaseMoverCharacter::OnDeathStarted(AActor*)
{
	DisableMovementAndCollision();
}

void ABaseMoverCharacter::OnDeathFinished(AActor*)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}


void ABaseMoverCharacter::DisableMovementAndCollision()
{
	if (GetController())
	{
		GetController()->SetIgnoreMoveInput(true);
	}
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
}

void ABaseMoverCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UnInitAndDestroy();
}


void ABaseMoverCharacter::UnInitAndDestroy()
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

