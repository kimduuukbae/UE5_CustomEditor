#pragma once

#include "CLMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FCLMappableConfigPair
{
	GENERATED_BODY()

public:
	FCLMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};