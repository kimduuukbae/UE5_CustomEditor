#pragma once

#include "GameFramework/PlayerState.h"
#include "CustomLyra/Character/CLPawnData.h"
#include "CLPlayerState.generated.h"

class UCLExperienceDefinition;

UCLASS()
class ACLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACLPlayerState(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	void OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition);

	void SetPawnData(TObjectPtr<const UCLPawnData> InPawnData);
	template <typename T>
	TObjectPtr<const T> GetPawnData() const {return Cast<const T>(PawnData); }

	UPROPERTY()
	TObjectPtr<const UCLPawnData> PawnData;
};