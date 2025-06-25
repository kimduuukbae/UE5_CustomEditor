#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.generated.h"

UCLASS()
class UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UGameFeatureAction_AddGameplayCuePath();

	UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};