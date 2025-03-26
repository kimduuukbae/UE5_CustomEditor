#pragma once

#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLPawnExtensionComponent;

UCLASS()
class ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& InObjectInitilaizer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CL|Character")
	TObjectPtr<UCLPawnExtensionComponent> PawnExtensionComponent;
};