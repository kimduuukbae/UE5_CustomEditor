#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "CLGameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct FCLGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

public:
	FCLGameplayAbilityTargetData_SingleTargetHit() : CartridgeId(-1) {}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FCLGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	UPROPERTY()
	int32 CartridgeId;
};