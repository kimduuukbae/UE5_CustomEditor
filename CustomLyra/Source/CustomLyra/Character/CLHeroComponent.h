#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "CLHeroComponent.generated.h"

struct FCLMappableConfigPair;
class UInputComponent;
struct FInputActionValue;
class UCLCameraMode;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UCLHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	static const FName NAME_HeroFeatureName;
	static const FName NAME_BindInputsNow;

	UCLHeroComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override;

	virtual FName GetFeatureName() const override { return NAME_HeroFeatureName; }
	virtual void CheckDefaultInitialization() override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& InParam) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;

	TSubclassOf<UCLCameraMode> DetermineCameraMode() const;
	void InitializePlayerInput(TObjectPtr<UInputComponent> InInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditAnywhere)
	TArray<FCLMappableConfigPair> DefaultInputConfigs;
};