// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"

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
#pragma endregion
