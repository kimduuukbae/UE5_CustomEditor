#pragma once

#include "GameFramework/PlayerState.h"
#include "CLPlayerState.generated.h"

class UCLPawnData;
class UCLExperienceDefinition;

UCLASS()
class ACLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACLPlayerState(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	void OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition);

	UPROPERTY()
	TObjectPtr<const UCLPawnData> PawnData;
};