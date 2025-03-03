// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLPawnData.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMLYRA_API UCLPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCLPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
