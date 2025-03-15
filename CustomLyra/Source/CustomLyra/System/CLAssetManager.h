#pragma once

#include "Engine/AssetManager.h"
#include "CLAssetManager.generated.h"

UCLASS()
class UCLAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UCLAssetManager();

	static UCLAssetManager& Get();

	void AddLoadedAsset(const TObjectPtr<UObject> Asset);
	static bool ShouldLogAssetLoads();

	// LoadAsset
	static TObjectPtr<UObject> SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	template <typename AssetType>
	static TObjectPtr<AssetType> GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// 액터는 BP Class 를 월드에 스폰한다는 개념, Actor를 만들때는 어떤 클래스를 가진 애를 스폰할 것인가에 대해서 에셋으로 들어가 있고
	// BP는 하나의 에셋이기 대문에, 다로 subclassOf라고 관리하고 있다.
	// 즉 Texture, StaticMesh, Blueprint 등등이 다 Asset이다. (즉 Asset과 Class)
	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	virtual void StartInitialLoading() override;

private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection AssetCS;
};

template <typename AssetType>
TObjectPtr<AssetType> UCLAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TObjectPtr<AssetType> loadedAsset = nullptr;
	const FSoftObjectPath& assetPath = AssetPointer->ToSoftObjectPath();
	if (assetPath.IsNull() == true)
	{
		loadedAsset = Cast<AssetType>(SynchronousLoadAsset(assetPath));
		ensureAlwaysMsgf(loadedAsset, TEXT("Failed to load asset : %s "), *AssetPointer.ToString());
	}

	if (IsValid(loadedAsset) == true && bKeepInMemory == true)
	{
		AddLoadedAsset(loadedAsset);
	}

	return loadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UCLAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> loadedSubclass = nullptr;
	const FSoftObjectPath& assetPath = AssetPointer->ToSoftObjectPath();
	if (assetPath.IsValid() == true)
	{
		loadedSubclass = AssetPointer.Get();
		if (loadedSubclass == nullptr)
		{
			loadedSubclass = Cast<UClass>(SynchronousLoadAsset(assetPath));
			ensureAlwaysMsgf(loadedSubclass, TEXT("Failed to load assetclass : %s "), *AssetPointer.ToString());
		}

		if (loadedSubclass != nullptr && bKeepInMemory == true)
		{
			AddLoadedAsset(Cast<UObject>(loadedSubclass));
		}
	}

	return loadedSubclass;
}