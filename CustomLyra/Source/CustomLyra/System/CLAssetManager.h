#pragma once

#include "Engine/AssetManager.h"
#include "CLAssetManager.generated.h"

UCLASS()
class UCLAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UCLAssetManager();

	static bool ShouldLogAssetLoads();
};