#include "CLGameFeaturePolicy.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "AbilitySystemGlobals.h"
#include "GameFeatureAction_AddGameplayCuePath.h"
#include "CustomLyra/AbilitySystem/CLGameplayCueManager.h"

UCLGameFeaturePolicy::UCLGameFeaturePolicy(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLGameFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<UCLGameFeature_AddGameplayCuePaths>());

	UGameFeaturesSubsystem& subSystem = UGameFeaturesSubsystem::Get();
	for (TObjectPtr<UObject> observer : Observers)
	{
		subSystem.AddObserver(observer);
	}

	Super::InitGameFeatureManager();
}

void UCLGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& subSystem = UGameFeaturesSubsystem::Get();
	for (TObjectPtr<UObject> observer : Observers)
	{
		subSystem.RemoveObserver(observer);
	}

	Observers.Empty();
}

void UCLGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString pluginRootPath = TEXT("/") + PluginName;

	for (const UGameFeatureAction* action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* addGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(action))
		{
			const TArray<FDirectoryPath>& dirToAdd = addGameplayCueGFA->DirectoryPathsToAdd;

			if (UCLGameplayCueManager* GCM = UCLGameplayCueManager::Get())
			{
				UGameplayCueSet* runtimeGameplayCueSet = GCM->GetRuntimeCueSet();
				int32 preInitializeNumCues = runtimeGameplayCueSet ? runtimeGameplayCueSet->GameplayCueData.Num() : 0;

				for (const FDirectoryPath& directory : dirToAdd)
				{
					FString mutablePath = directory.Path;

					UGameFeaturesSubsystem::FixPluginPackagePath(mutablePath, pluginRootPath, false);

					GCM->AddGameplayCueNotifyPath(mutablePath);
				}

				if (dirToAdd.IsEmpty() == false)
				{
					GCM->InitializeRuntimeObjectLibrary();
				}

				int32 postInitializeNumCues = runtimeGameplayCueSet ? runtimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (preInitializeNumCues != postInitializeNumCues)
				{
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void UCLGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString pluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* addGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(GameFeatureData))
		{
			const TArray<FDirectoryPath>& dirToAdd = addGameplayCueGFA->DirectoryPathsToAdd;
			if (UCLGameplayCueManager* GCM = UCLGameplayCueManager::Get())
			{
				int32 numRemoved = 0;
				for (const FDirectoryPath& directory : dirToAdd)
				{
					FString mutablePath = directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(mutablePath, pluginRootPath, false);
					numRemoved += GCM->RemoveGameplayCueNotifyPath(mutablePath, false);
				}

				if (numRemoved > 0)
				{
					GCM->InitializeRuntimeObjectLibrary();
				}
			}
		}
	}
}
