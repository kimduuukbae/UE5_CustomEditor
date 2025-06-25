#pragma once

#include "GameplayCueManager.h"
#include "CLGameplayCueManager.generated.h"

UCLASS()
class UCLGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	static UCLGameplayCueManager* Get();

	UCLGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void RefreshGameplayCuePrimaryAsset();
};