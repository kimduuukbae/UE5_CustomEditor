#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "CLPawnExtensionComponent.generated.h"

UCLASS()
class UCLPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UCLPawnExtensionComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;
};