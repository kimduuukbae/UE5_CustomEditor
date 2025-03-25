#pragma once

#include "Engine/GameInstance.h"
#include "CLGameInstance.generated.h"

UCLASS()
class UCLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCLGameInstance(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;
	virtual void Shutdown() override;
};