// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSuperManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#pragma region ContentBrowserMenuExtention
	void InitCBMenuExtention();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);
	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);

	void OnAdvanceDeletionButtonClicked();

	void OnDeleteUnusedAssetButtonClicked();
	void FixUpRedirectors();

	TArray<FString> FolderPathsSelected;
#pragma endregion

#pragma region CustomEditorTab

	void RegisterAdvanceDeletionTab();

	TSharedRef<SDockTab> OnSpawnAdvanceDeletionTab(const FSpawnTabArgs& SpawnTab);
	TArray<TSharedPtr<FAssetData>> GetAllAssetDataUnderSelectedFolder();

#pragma endregion

public:

#pragma region ProcessDataForAssetList
	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);
	bool DeleteMultipleAssetForAssetList(const TArray<FAssetData>& AssetsDataToDelete);
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetData);
	void SyncContentBrowser(const FString& AssetPath);

#pragma endregion


#pragma region SelectionLock

public:
	void InitCustomSelectionEvent();
	void OnActorSelected(UObject* SelectedObject);

	void LockActor(AActor* Target);
	void UnLockActor(AActor* Target);
	
private:
	TWeakObjectPtr<class UEditorActorSubsystem> ActorSubsystem;
	bool GetActorSubsystem();

#pragma endregion

#pragma region SceneOutlinerExtension
public:
	void InitSceneOutlinerColumnExtension();

#pragma endregion

	bool CheckIsActorLocked(AActor* Target);
	void ProcessLockingForOutliner(AActor* Target, bool bLock);
};
