#pragma once

#include "GameFramework/PlayerController.h"
#include "CLPlayerController.generated.h"

UCLASS()
class ACLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACLPlayerController(const FObjectInitializer& InInitializer = FObjectInitializer::Get());
};