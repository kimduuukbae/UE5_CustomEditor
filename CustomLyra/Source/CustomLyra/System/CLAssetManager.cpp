#include "CLAssetManager.h"
#include "CustomLyra/CLLogChannels.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CLAssetManager)

UCLAssetManager::UCLAssetManager() : Super()
{
	
}

UCLAssetManager& UCLAssetManager::Get()
{
	if (TObjectPtr<UCLAssetManager> assetManager = Cast<UCLAssetManager>(GEngine->AssetManager))
	{
		return *assetManager;
	}

	UE_LOG(LogCL, Fatal, TEXT("Invalid AssetManager"));

	return *NewObject<UCLAssetManager>();
}

void UCLAssetManager::AddLoadedAsset(const TObjectPtr<UObject> Asset)
{
	if (Asset->IsValidLowLevel() == false)
	{
		return;
	}

	FScopeLock lock{ &AssetCS };
	LoadedAssets.Add(Asset);
}

bool UCLAssetManager::ShouldLogAssetLoads()
{
	/*
		VITCHAN, FCommandLine::Get() 으로 현재 실행시킬때 CommandLine을 가져올 수 있다.
	*/
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

TObjectPtr<UObject> UCLAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid() == true)
	{
		TUniquePtr<FScopeLogTime> logTimePtr;
		if (ShouldLogAssetLoads() == true)
		{
			logTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Syncronous loaded asset : %s"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid() == true)
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// StaticLoadObject -> StaticLoadObjectInternal -> StaticFindObjectFast (GC로 관리되고 있는 오브젝트라면 바로 반환함)
		return AssetPath.TryLoad();
	}

	return TObjectPtr<UObject>(nullptr);
}

void UCLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
