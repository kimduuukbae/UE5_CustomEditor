#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS(BlueprintType)
class UCLExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UCLExperienceActionSet();

	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};