// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CLGameMode.generated.h"

class UCLExperienceDefinition;

UCLASS()
class CUSTOMLYRA_API ACLGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACLGameMode(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameState() override;
	virtual void StartPlay() override;

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer);

	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition);
};
