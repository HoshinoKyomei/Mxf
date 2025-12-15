// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Components/PawnComponent.h"

#include "BasePawnComponent.generated.h"

enum class ETriggerEvent : uint8;
class UBaseInputConfig;
class UGameFrameworkComponentManager;
class UInputComponent;
class UObject;
struct FActorInitStateChangedParams;
struct FFrame;
struct FGameplayTag;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FBaseGameFrameworkSet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	bool IsEnabled = true;
	
	UPROPERTY(EditDefaultsOnly)
	FName FeatureName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RequiredStateTags = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly)
	bool IsCallIfReached =  false;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ToStateTags = FGameplayTag();
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class MXFGAME_API UBasePawnComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	
	UBasePawnComponent(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DeterminesOutputType="ComponentClass"))
	UPawnComponent* FindPawnComponent(const AActor* Actor, const TSubclassOf<UPawnComponent> ComponentClass) const;
	
	//~ Check Pawn Interface
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPawnChecked"))
	APawn* K2_GetPawnChecked() const { return GetPawnChecked<APawn>(); }
	
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPawn"))
	APawn* K2_GetPawn() const { return GetPawn<APawn>(); }
	
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPlayerState"))
	APlayerState* K2_GetPlayerState() const { return GetPlayerState<APlayerState>(); }
	
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DisplayName="GetController"))
	AController* K2_GetController() const { return GetController<AController>(); }
	
	UFUNCTION(BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPlayerController"))
	APlayerController* K2_GetPlayerController() const { return GetController<APlayerController>(); }
	//~ End Get Pawn Interface
	
	/** The name of this overall feature, this one depends on the other named component features */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Default")
	FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface
	
	/** Returns true if feature has reached query state or later */
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="HasFeatureReachedInitState"))
	bool K2_HasFeatureReachedInitState(UGameFrameworkComponentManager* Manager, AActor* Actor, FName FeatureName, FGameplayTag FeatureState) const;

protected:
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="ConstructionScript"))
	bool K2_ConstructionScript();
	
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="OnRegister"))
	void K2_OnRegister();
	
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	
	// Native events
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="CanChangeInitState"))
	bool K2_CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="HandleChangeInitState"))
	bool K2_HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="OnActorInitStateChanged"))
	bool K2_OnActorInitStateChanged(const FActorInitStateChangedParams& Params);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="CheckDefaultInitialization"))
	bool K2_CheckDefaultInitialization();
	
	//Blueprint events
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="RegisterInitStateFeature"))
	void K2_RegisterInitStateFeature();
	
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="CheckDefaultInitializationForImplementers"))
	void K2_CheckDefaultInitializationForImplementers();
	
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="ContinueInitStateChain"))
	void K2_ContinueInitStateChain();
	
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="BindOnActorInitStateChanged"))
	void K2_BindOnActorInitStateChanged(FBaseGameFrameworkSet InGameFrameworkSet);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameFramework", meta=(DisplayName="HaveAllFeaturesReachedInitState"))
	bool K2_HaveAllFeaturesReachedInitState(UGameFrameworkComponentManager* Manager, AActor* Actor, FGameplayTag RequiredState, FName ExcludingFeature = NAME_None) const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Default")
	FBaseGameFrameworkSet GameFrameworkSet;
};
