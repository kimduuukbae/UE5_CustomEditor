#pragma once

#include "Abilities/GameplayAbility.h"
#include "CLGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ECLAbilityActivationPolicy : uint8
{
	OnInputTriggered = 0,
	WhileInputActive,
	OnSpawn
};

UCLASS(Abstract)
class UCLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCLGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CL|AbilityActivation")
	ECLAbilityActivationPolicy ActivationPolicy = ECLAbilityActivationPolicy::OnInputTriggered;
};