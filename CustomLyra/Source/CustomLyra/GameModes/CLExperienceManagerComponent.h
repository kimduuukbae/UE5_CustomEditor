#pragma once

#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "CLExperienceManagerComponent.generated.h"

class UCLExperienceDefinition;

enum class ECLExperienceLoadState : uint8
{
	UnLoaded = 0,
	Loading,
	LoadingGameFeatures,
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

	void ServerSetCurrentExperience(const FPrimaryAssetId& InExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& InResult);
	void OnExperienceFullLoadComplete();

	TObjectPtr<const UCLExperienceDefinition> GetCurrentExperienceChecked();
private:
	UPROPERTY()
	TObjectPtr<const UCLExperienceDefinition> CurrentExperience = nullptr;

	FCLOnExperienceLoaded OnExperienceLoaded;

	ECLExperienceLoadState LoadState = ECLExperienceLoadState::UnLoaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};