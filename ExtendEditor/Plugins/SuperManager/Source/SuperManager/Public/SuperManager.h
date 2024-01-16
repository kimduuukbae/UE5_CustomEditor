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
};
