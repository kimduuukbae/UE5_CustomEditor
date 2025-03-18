#pragma once

#include "GameFramework/PlayerState.h"
#include "CLPlayerState.generated.h"

UCLASS()
class ACLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACLPlayerState(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
};