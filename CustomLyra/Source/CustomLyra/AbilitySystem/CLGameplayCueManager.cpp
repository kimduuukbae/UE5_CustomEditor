#include "CLGameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "CustomLyra/System/CLAssetManager.h"

UCLGameplayCueManager::UCLGameplayCueManager(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{

}

UCLGameplayCueManager* UCLGameplayCueManager::Get()
{
	return Cast<UCLGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

void UCLGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> cuePaths;
	UGameplayCueSet* runtimeGameplayCueSet = GetRuntimeCueSet();
	if (runtimeGameplayCueSet)
	{
		runtimeGameplayCueSet->GetSoftObjectPaths(cuePaths);
	}

	FAssetBundleData bundleData;
	bundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, cuePaths);

	FPrimaryAssetId primaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

	UAssetManager::Get().AddDynamicAsset(primaryAssetId, FSoftObjectPath(), bundleData);
}
