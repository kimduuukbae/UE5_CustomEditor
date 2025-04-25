#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "CLPawnExtensionComponent.generated.h"

class UCLPawnData;

UCLASS()
class UCLPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UCLPawnExtensionComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	static TObjectPtr<UCLPawnExtensionComponent> FindPawnExtensionComponent(const TObjectPtr<AActor> InActor)
	{
		return (IsValid(InActor) ? InActor->FindComponentByClass<UCLPawnExtensionComponent>() : nullptr);
	}

	void SetPawnData(TObjectPtr<const UCLPawnData> InPawnData);
	template <typename T>
	TObjectPtr<const T> GetPawnData() const { return Cast<T>(PawnData); }

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static const FName NAME_ActorFeatureName;
	virtual FName GetFeatureName() const { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void CheckDefaultInitialization() override;

	void SetupPlayerInputComponent();

private:
	UPROPERTY(EditDefaultsOnly, Category = "CL|Pawn")
	TObjectPtr<const UCLPawnData> PawnData;
};