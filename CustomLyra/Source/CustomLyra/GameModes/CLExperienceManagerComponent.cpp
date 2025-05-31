#include "CLExperienceManagerComponent.h"
#include "CLExperienceDefinition.h"
#include "CustomLyra/System/CLAssetManager.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"

UCLExperienceManagerComponent::UCLExperienceManagerComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

void UCLExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate&& InDelegate)
{
	if (IsExperienceLoaded())
	{
		InDelegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(InDelegate));
	}
}

void UCLExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& InExperienceId)
{
	UCLAssetManager& assetManager = UCLAssetManager::Get();

	TSubclassOf<UCLExperienceDefinition> assetClass;
	{
		FSoftObjectPath assetPath = assetManager.GetPrimaryAssetPath(InExperienceId);
		assetClass = Cast<UClass>(assetPath.TryLoad());
	}

	TObjectPtr<const UCLExperienceDefinition> experience = GetDefault<UCLExperienceDefinition>(assetClass);
	if (IsValid(experience) == false)
	{
		return;
	}

	CurrentExperience = experience;
	StartExperienceLoad();
}

void UCLExperienceManagerComponent::StartExperienceLoad()
{
	if (IsValid(CurrentExperience) == false)
	{
		return;
	}

	if (LoadState != ECLExperienceLoadState::UnLoaded)
	{
		return;
	}
	
	LoadState = ECLExperienceLoadState::Loading;

	UCLAssetManager& assetManager = UCLAssetManager::Get();
	TSet<FPrimaryAssetId> bundleAssetList;

	bundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> bundlesToLoad;
	{
		const ENetMode ownerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || ownerNetMode != NM_DedicatedServer;
		bool bLoadServer = GIsEditor || ownerNetMode != NM_Client;
		if (bLoadClient == true)
		{
			bundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer == true)
		{
			bundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate onAssetLoadedDelegate = FStreamableDelegate::CreateUObject(this, &UCLExperienceManagerComponent::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> handle = assetManager.ChangeBundleStateForPrimaryAssets(
		bundleAssetList.Array(),
		bundlesToLoad,
		{}, false, FStreamableDelegate{}, FStreamableManager::AsyncLoadHighPriority);

	if (handle.IsValid() == false || handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(onAssetLoadedDelegate);
	}
	else
	{
		handle->BindCompleteDelegate(onAssetLoadedDelegate);
		handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([onAssetLoadedDelegate]()
			{
				onAssetLoadedDelegate.ExecuteIfBound();
			}));
	}

	static int32 startExperienceLoad_FrameNumber = GFrameNumber;
}

void UCLExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 onExperienceLoadComplete_FrameNumber = GFrameNumber;

	GameFeaturePluginURLs.Reset();

	auto collectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			for (const FString& pluginName : FeaturePluginList)
			{
				FString pluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(pluginName, pluginURL) == true)
				{
					This->GameFeaturePluginURLs.AddUnique(pluginURL);
				}
			}
		};

	// GameFEaturesToEnable에 있는 plugin들만 일단 활성화
	collectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// plugin 로딩 및 활성화
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading > 0)
	{
		LoadState = ECLExperienceLoadState::LoadingGameFeatures;
		for (const FString& pluginURL : GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(pluginURL,
				FGameFeaturePluginLoadComplete::CreateUObject(this, &UCLExperienceManagerComponent::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadComplete();
	}
}

void UCLExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& InResult)
{
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		OnExperienceFullLoadComplete();
	}
}

void UCLExperienceManagerComponent::OnExperienceFullLoadComplete()
{
	if (LoadState != ECLExperienceLoadState::Loading)
	{
		return;
	}

	LoadState = ECLExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

TObjectPtr<const UCLExperienceDefinition> UCLExperienceManagerComponent::GetCurrentExperienceChecked()
{
	return CurrentExperience;
}
