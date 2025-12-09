// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "BasePawnComponent.generated.h"

class UGameFrameworkComponentManager;
class UObject;
struct FActorInitStateChangedParams;
struct FFrame;
struct FGameplayTag;

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
	
	//~ Check Pawn Interface
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPawnChecked"))
	APawn* GetBasePawnChecked() const { return GetPawnChecked<APawn>(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameFramework", meta=(DisplayName="GetPawn"))
	APawn* GetBasePawn() const { return GetPawn<APawn>(); }
	//~ End Check Pawn Interface
	
	//~ Get Pawn Interface
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameFramework", meta=(DisplayName="GetController"))
	AController* GetBaseController() const { return GetController<AController>(); }
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
