#pragma once

#include "Components/GameStateComponent.h"
#include "CLExperienceManagerComponent.generated.h"

class UCLExperienceDefinition;

enum class ECLExperienceLoadState : uint8
{
	UnLoaded = 0,
	Loading,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FCLOnExperienceLoaded, TObjectPtr<const UCLExperienceDefinition>);

UCLASS()
class UCLExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UCLExperienceManagerComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() { return LoadState == ECLExperienceLoadState::Loaded && CurrentExperience != nullptr; }

	void CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate&& InDelegate);
private:
	UPROPERTY()
	TObjectPtr<const UCLExperienceDefinition> CurrentExperience = nullptr;

	FCLOnExperienceLoaded OnExperienceLoaded;

	ECLExperienceLoadState LoadState = ECLExperienceLoadState::UnLoaded;
};