#pragma once

#include "Abilities/GameplayAbility.h"
#include "CLGameplayAbility.generated.h"

UCLASS(Abstract)
class UCLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCLGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};