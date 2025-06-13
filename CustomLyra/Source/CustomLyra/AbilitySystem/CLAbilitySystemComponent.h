#pragma once

#include "AbilitySystemComponent.h"
#include "CLAbilitySystemComponent.generated.h"

UCLASS()
class UCLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCLAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};