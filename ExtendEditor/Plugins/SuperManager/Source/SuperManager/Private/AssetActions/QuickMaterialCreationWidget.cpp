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
	uint32 ConnectedCounter = 0;

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

	for (UTexture2D* SelectedTexture : SelectedTexturesArray)
	{
		if (SelectedTexture == nullptr)
		{
			continue;
		}

		DefaultCreateMaterialNodes(CreatedMaterial, SelectedTexture, ConnectedCounter);
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
void UQuickMaterialCreationWidget::DefaultCreateMaterialNodes(UMaterial* Material, UTexture2D* Texture, uint32& OutConnectedPinNumber)
{
	UMaterialExpressionTextureSample* TextureSampleNode = NewObject<UMaterialExpressionTextureSample>(Material);

	if (TextureSampleNode == nullptr)
	{
		return;
	}

	/*	Material BaseColor have been made private in 5.1
		So, We need to use something like this : 
		
		Material->GetExpressionCollection().AddExpression(TextureSampleNode);
		- Instead of directly accessing the array Expression, getter function then use to add in the texture sample node 

		Material->GetExpressionInputForProperty(MP_BaseColor)->Connect(0, TextureSampleNode);
		- no longer have direct access 

		This means that you will connect a new Expression (Texture) and then connect it to the BaseColor Node in Material.
	*/

	if (TryConnectBaseColor(TextureSampleNode, Texture, Material) == true)
	{
		++OutConnectedPinNumber;
		return;
	}

	if (TryConnectMetalic(TextureSampleNode, Texture, Material) == true)
	{
		++OutConnectedPinNumber;
		return;
	}

	if (TryConnectRoughness(TextureSampleNode, Texture, Material) == true)
	{
		++OutConnectedPinNumber;
		return;
	}

	if (TryConnectNormal(TextureSampleNode, Texture, Material) == true)
	{
		++OutConnectedPinNumber;
		return;
	}

	if (TryConnectAmbientOcclusion(TextureSampleNode, Texture, Material) == true)
	{
		++OutConnectedPinNumber;
		return;
	}

	MaterialName = TEXT("M_");
}
#pragma endregion

#pragma region CreateMaterialNodes
bool UQuickMaterialCreationWidget::TryConnectBaseColor(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material)
{
	if (CheckNodeConnected(Material, EMaterialProperty::MP_BaseColor))
	{
		return false;
	}

	for (const FString& BaseColorName : BaseColorArray)
	{
		if (Texture->GetName().Contains(BaseColorName))
		{
			SampleNode->Texture = Texture;

			Material->GetExpressionCollection().AddExpression(SampleNode);
			Material->GetExpressionInputForProperty(EMaterialProperty::MP_BaseColor)->Connect(0, SampleNode);
			Material->PostEditChange();

			SampleNode->MaterialExpressionEditorX -= 600;

			return true;
		}
	}

	return false;
}
bool UQuickMaterialCreationWidget::TryConnectMetalic(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material)
{
	if (CheckNodeConnected(Material, EMaterialProperty::MP_Metallic))
	{
		return false;
	}

	for (const FString& MetalicName : MetalicArray)
	{
		if (Texture->GetName().Contains(MetalicName))
		{

			// Metalic SRGB is false 
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;
			Texture->SRGB = false;
			Texture->PostEditChange();

			SampleNode->Texture = Texture;
			SampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			Material->GetExpressionCollection().AddExpression(SampleNode);
			Material->GetExpressionInputForProperty(EMaterialProperty::MP_Metallic)->Connect(0, SampleNode);
			Material->PostEditChange();

			SampleNode->MaterialExpressionEditorX -= 600;
			SampleNode->MaterialExpressionEditorY += 240;

			return true;
		}
	}

	return false;
}
bool UQuickMaterialCreationWidget::TryConnectRoughness(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material)
{
	if (CheckNodeConnected(Material, EMaterialProperty::MP_Roughness))
	{
		return false;
	}

	for (const FString& RoughnessName : RoughnessArray)
	{
		if (Texture->GetName().Contains(RoughnessName))
		{
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;
			Texture->SRGB = false;
			Texture->PostEditChange();

			SampleNode->Texture = Texture;
			SampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			Material->GetExpressionCollection().AddExpression(SampleNode);
			Material->GetExpressionInputForProperty(EMaterialProperty::MP_Roughness)->Connect(0, SampleNode);
			Material->PostEditChange();

			SampleNode->MaterialExpressionEditorX -= 600;
			SampleNode->MaterialExpressionEditorY += 480;

			return true;
		}
	}

	return false;
}
bool UQuickMaterialCreationWidget::TryConnectNormal(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material)
{
	if (CheckNodeConnected(Material, EMaterialProperty::MP_Normal))
	{
		return false;
	}

	for (const FString& NormalName : NormalArray)
	{
		if (Texture->GetName().Contains(NormalName))
		{
			SampleNode->Texture = Texture;
			SampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_Normal;

			Material->GetExpressionCollection().AddExpression(SampleNode);
			Material->GetExpressionInputForProperty(EMaterialProperty::MP_Normal)->Connect(0, SampleNode);
			Material->PostEditChange();

			SampleNode->MaterialExpressionEditorX -= 600;
			SampleNode->MaterialExpressionEditorY += 720;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectAmbientOcclusion(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material)
{
	if (CheckNodeConnected(Material, EMaterialProperty::MP_AmbientOcclusion))
	{
		return false;
	}

	for (const FString& AOName : AOArray)
	{
		if (Texture->GetName().Contains(AOName))
		{
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;
			Texture->SRGB = false;
			Texture->PostEditChange();

			SampleNode->Texture = Texture;
			SampleNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			Material->GetExpressionCollection().AddExpression(SampleNode);
			Material->GetExpressionInputForProperty(EMaterialProperty::MP_AmbientOcclusion)->Connect(0, SampleNode);
			Material->PostEditChange();

			SampleNode->MaterialExpressionEditorX -= 600;
			SampleNode->MaterialExpressionEditorY += 960;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::CheckNodeConnected(UMaterial* Material, EMaterialProperty Property)
{
	return Material->GetExpressionInputForProperty(Property)->IsConnected();
}
#pragma endregion
