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
	
	/** The name of this overall feature, this one depends on the other named component features */
	UPROPERTY(EditDefaultsOnly, Category="GameFramework")
	FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	bool BaseConstructionScript();
	
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	
	// Native events
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="CanChangeInitState"))
	bool GameFramework_CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const;
	
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="HandleChangeInitState"))
	bool GameFramework_HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState);
	
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="OnActorInitStateChanged"))
	bool GameFramework_OnActorInitStateChanged(const FActorInitStateChangedParams& Params);
	
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="CheckDefaultInitialization"))
	bool GameFramework_CheckDefaultInitialization();
	
	UFUNCTION(BlueprintNativeEvent, Category="GameFramework", meta=(DisplayName="OnRegisterInitFeature"))
	bool GameFramework_OnRegisterInitFeature();
	
	//Blueprint events
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="Call_RegisterInitStateFeature"))
	void GameFramework_CallRegisterInitStateFeature();
	
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="Call_ContinueInitStateChain"))
	void GameFramework_CallContinueInitStateChain(TArray<FGameplayTag> InitStateChain);
	
	UFUNCTION(BlueprintCallable, Category="GameFramework", meta=(DisplayName="Call_BindOnActorInitStateChanged"))
	void GameFramework_CallBindOnActorInitStateChanged(FName FeatureName, FBaseGameFrameworkSet InGameFrameworkSet);
	
	UPROPERTY(EditDefaultsOnly, Category="GameFramework")
	FBaseGameFrameworkSet GameFrameworkSet;
};
