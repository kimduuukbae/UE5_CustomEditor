#pragma once

#include "ModularPlayerController.h"
#include "CLPlayerController.generated.h"

UCLASS()
class ACLPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACLPlayerController(const FObjectInitializer& InInitializer = FObjectInitializer::Get());
};