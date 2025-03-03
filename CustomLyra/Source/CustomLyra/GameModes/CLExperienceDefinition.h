// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLExperienceDefinition.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMLYRA_API UCLExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TObjectPtr<class UCLPawnData> DefaultPawnData = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FString> GameFeaturesToEnable;
};
