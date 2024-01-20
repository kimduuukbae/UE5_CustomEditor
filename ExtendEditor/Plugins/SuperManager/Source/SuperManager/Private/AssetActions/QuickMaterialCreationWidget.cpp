// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"

#pragma region QuickMaterialCreationCore
void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	if (bCustomMaterialName)
	{
		if (MaterialName.IsEmpty() || MaterialName.Equals(TEXT("M_")))
		{
			DebugHeader::ShowMessageDialog(EAppMsgType::Ok, TEXT("Please enter a valid name"));
			return;
		}
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<UTexture2D*> SelectedTexturesArray;
	FString PackagePath;

	if (ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, PackagePath) == false)
	{
		return;
	}

	if (IsNameUsed(PackagePath, MaterialName))
	{
		return;
	}

	UMaterial* CreatedMaterial = CreateMaterialAsset(MaterialName, PackagePath);

	if (CreatedMaterial == nullptr)
	{
		DebugHeader::ShowMessageDialog(EAppMsgType::Ok, TEXT("Failed to create material"));
		return;
	}
}
#pragma endregion

#pragma region QuickMaterialCreation
bool UQuickMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess, TArray<UTexture2D*>& OutResult, FString& OutSelectedTexturePackagePath)
{
	if (SelectedDataToProcess.Num() == 0)
	{
		DebugHeader::ShowMessageDialog(EAppMsgType::Ok, TEXT("No texture selected"));
		return false;
	}

	bool bMaterialNameSet = false;
	for (const FAssetData& Data : SelectedDataToProcess)
	{
		UObject* SelectedAsset = Data.GetAsset();

		if (SelectedAsset == nullptr)
		{
			continue;
		}

		UTexture2D* Texture = Cast<UTexture2D>(SelectedAsset);

		if (Texture == nullptr)
		{
			return false;
		}

		OutResult.Add(Texture);

		if (OutSelectedTexturePackagePath.IsEmpty())
		{
			OutSelectedTexturePackagePath = Data.PackagePath.ToString();
		}

		if (bCustomMaterialName == false && bMaterialNameSet == false)
		{
			MaterialName = SelectedAsset->GetName();
			MaterialName.RemoveFromStart(TEXT("T_"));
			MaterialName.InsertAt(0, TEXT("M_"));

			bMaterialNameSet = true;
		}
	}

	return true;
}
bool UQuickMaterialCreationWidget::IsNameUsed(const FString& FolderPath, const FString& TargetMaterialName)
{
	TArray<FString> ExistingAssetsPath = UEditorAssetLibrary::ListAssets(FolderPath, false);

	for (const FString& AssetPath : ExistingAssetsPath)
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(AssetPath);

		if (ExistingAssetName.Equals(TargetMaterialName))
		{
			return true;
		}
	}

	return false;
}
UMaterial* UQuickMaterialCreationWidget::CreateMaterialAsset(const FString& CreateMaterialName, const FString& Path)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	
	UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();

	UObject* CreatedObject = AssetToolsModule.Get().CreateAsset(CreateMaterialName, Path, UMaterial::StaticClass(), MaterialFactory);

	return Cast<UMaterial>(CreatedObject);
}
#pragma endregion
