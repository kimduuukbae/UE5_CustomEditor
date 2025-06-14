#pragma once

#include "ModularPlayerController.h"
#include "CLPlayerController.generated.h"

class ACLPlayerState;
class UCLAbilitySystemComponent;

UCLASS()
class ACLPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACLPlayerController(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	ACLPlayerState* GetCLPlayerState() const;
	UCLAbilitySystemComponent* GetCLAbilitySystemComponent() const;
};