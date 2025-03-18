// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CLGameMode.generated.h"

UCLASS()
class CUSTOMLYRA_API ACLGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACLGameMode(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void StartPlay() override;
};
