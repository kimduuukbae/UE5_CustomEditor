#pragma once

#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

UCLASS()
class ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& InObjectInitilaizer = FObjectInitializer::Get());
};