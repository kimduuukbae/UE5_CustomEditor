// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLExperienceDefinition.generated.h"

class UCLPawnData;
class UCLExperienceActionSet;
class UGameFeatureAction;

UCLASS()
class CUSTOMLYRA_API UCLExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TObjectPtr<class UCLPawnData> DefaultPawnData = nullptr;

	// 게임 모드에 따른 GameFeature Plugin을 로딩하는데, 이에 대한 연결고리다.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FString> GameFeaturesToEnable;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<UCLExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
