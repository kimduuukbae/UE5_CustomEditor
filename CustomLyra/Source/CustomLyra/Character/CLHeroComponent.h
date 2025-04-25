#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "CLHeroComponent.generated.h"

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UCLHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

	static const FName NAME_HeroFeatureName;
public:
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
};