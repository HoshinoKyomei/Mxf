// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseHeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "BaseLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/BasePlayerController.h"
#include "Player/BasePlayerState.h"
// #include "Player/BaseLocalPlayer.h"
#include "Character/BasePawnExtensionComponent.h"
#include "Character/BasePawnData.h"
#include "Character/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Input/BaseInputConfig.h"
#include "Input/BaseInputComponent.h"
#include "BaseGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace BaseHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UBaseHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UBaseHeroComponent::NAME_ActorFeatureName("Hero");

UBaseHeroComponent::UBaseHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

void UBaseHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogBase, Error, TEXT("[UBaseHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("BaseHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("BaseHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UBaseHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == BaseGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == BaseGameplayTags::InitState_Spawned && DesiredState == BaseGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<ABasePlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ABasePlayerController* BasePC = GetController<ABasePlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !BasePC || !BasePC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == BaseGameplayTags::InitState_DataAvailable && DesiredState == BaseGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ABasePlayerState* BasePS = GetPlayerState<ABasePlayerState>();

		return BasePS && Manager->HasFeatureReachedInitState(Pawn, UBasePawnExtensionComponent::NAME_ActorFeatureName, BaseGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == BaseGameplayTags::InitState_DataInitialized && DesiredState == BaseGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UBaseHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == BaseGameplayTags::InitState_DataAvailable && DesiredState == BaseGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ABasePlayerState* BasePS = GetPlayerState<ABasePlayerState>();
		if (!ensure(Pawn && BasePS))
		{
			return;
		}

		const UBasePawnData* PawnData = nullptr;

		if (UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UBasePawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(BasePS->GetBaseAbilitySystemComponent(), BasePS);
		}

		if (ABasePlayerController* BasePC = GetController<ABasePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			// CameraMode
		}
	}
}

void UBaseHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UBasePawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == BaseGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all other components are initialized, try to progress to the next state
			CheckDefaultInitialization();
		}
	}
}

void UBaseHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { BaseGameplayTags::InitState_Spawned, BaseGameplayTags::InitState_DataAvailable, BaseGameplayTags::InitState_DataInitialized, BaseGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UBaseHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UBasePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(BaseGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UBaseHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UBaseHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// const UBaseLocalPlayer* LP = Cast<UBaseLocalPlayer>(PC->GetLocalPlayer());
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UBasePawnData* PawnData = PawnExtComp->GetPawnData<UBasePawnData>())
		{
			if (const UBaseInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}
							
							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Base Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UBaseInputComponent or modify this component accordingly.
				UBaseInputComponent* BaseIC = Cast<UBaseInputComponent>(PlayerInputComponent);
				if (ensureMsgf(BaseIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UBaseInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					BaseIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					BaseIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					BaseIC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				
					// 遍历执行表单数据初始化绑定
					for (const UBaseInputSet* InputSet : PawnData->InputSets)
					{
						if (InputSet)
						{
							BaseIC->BindFunctionActions(PawnData->InputConfig, const_cast<APawn*>(Pawn), InputSet);
						}
					}
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UBaseHeroComponent::AddAdditionalInputConfig(const UBaseInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UBaseInputComponent* BaseIC = Pawn->FindComponentByClass<UBaseInputComponent>();
		if (ensureMsgf(BaseIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UBaseInputComponent or a subclass of it.")))
		{
			BaseIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UBaseHeroComponent::RemoveAdditionalInputConfig(const UBaseInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UBaseHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UBaseHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UBaseAbilitySystemComponent* BaseASC = PawnExtComp->GetBaseAbilitySystemComponent())
			{
				BaseASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UBaseHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UBasePawnExtensionComponent* PawnExtComp = UBasePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UBaseAbilitySystemComponent* BaseASC = PawnExtComp->GetBaseAbilitySystemComponent())
		{
			BaseASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UBaseHeroComponent::Input_Move_Implementation(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (ABasePlayerController* BaseController = Cast<ABasePlayerController>(Controller))
	{
		BaseController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UBaseHeroComponent::Input_LookMouse_Implementation(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UBaseHeroComponent::Input_LookStick_Implementation(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * BaseHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * BaseHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UBaseHeroComponent::Input_Crouch_Implementation(const FInputActionValue& InputActionValue)
{
	if (ABaseCharacter* Character = GetPawn<ABaseCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UBaseHeroComponent::Input_AutoRun_Implementation(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (ABasePlayerController* Controller = Cast<ABasePlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}
