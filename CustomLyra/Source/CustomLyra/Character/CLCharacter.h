#pragma once

#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLPawnExtensionComponent;
class UCLCameraComponent;

UCLASS()
class ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& InObjectInitilaizer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CL|Character")
	TObjectPtr<UCLPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CL|Character")
	TObjectPtr<UCLCameraComponent> CameraComponent;
};