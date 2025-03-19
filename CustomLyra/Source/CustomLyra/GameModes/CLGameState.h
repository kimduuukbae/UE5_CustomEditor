#pragma once

#include "GameFramework/GameStateBase.h"
#include "CLGameState.generated.h"

class UCLExperienceManagerComponent;

UCLASS()
class ACLGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACLGameState(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<UCLExperienceManagerComponent> ExperienceManagerComponent = nullptr;
};