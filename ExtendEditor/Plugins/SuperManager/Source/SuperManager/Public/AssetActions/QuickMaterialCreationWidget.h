// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
#pragma region QuickMaterialCreationCore
	UFUNCTION(BlueprintCallable)
	void CreateMaterialFromSelectedTextures();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bCustomMaterialName"))
	FString MaterialName = TEXT("M_");

#pragma endregion

#pragma region SupportedTextureNames
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture names")
		TArray<FString> BaseColorArray =
	{
		TEXT("_BaseColor"),
		TEXT("_Albedo"),
		TEXT("_Diffuse"),
		TEXT("_diff")
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture names")
	TArray<FString> MetallicArray =
	{
		TEXT("_Metallic"),
		TEXT("_metal"),
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture names")
	TArray<FString> RoughnessArray =
	{
		TEXT("_Roughness"),
		TEXT("_RoughnessMap"),
		TEXT("_rough"),
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture names")
	TArray<FString> NormalArray =
	{
		TEXT("_Normal"),
		TEXT("_NormalMap"),
		TEXT("_nor"),
		TEXT("_N"),
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture names")
	TArray<FString> AOArray =
	{
		TEXT("_AmbientOcclusion"),
		TEXT("_AmbientOcclusionMap"),
		TEXT("_AO"),
	};
#pragma endregion


private:

#pragma region QuickMaterialCreation
	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess, TArray<UTexture2D*>& OutResult, FString& OutSelectedTexturePackagePath);
	bool IsNameUsed(const FString& FolderPath, const FString& TargetMaterialName);
	UMaterial* CreateMaterialAsset(const FString& CreateMaterialName, const FString& Path);

	void DefaultCreateMaterialNodes(UMaterial* Material, UTexture2D* Texture, uint32& OutConnectedPinNumber);
#pragma endregion

#pragma region CreateMaterialNodes
	bool TryConnectBaseColor(UMaterialExpressionTextureSample* SampleNode, UTexture2D* Texture, UMaterial* Material);
#pragma endregion


};
