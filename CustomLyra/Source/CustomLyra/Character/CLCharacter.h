#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "CLCharacter.generated.h"

class UCLPawnExtensionComponent;
class UCLCameraComponent;

UCLASS()
class ACLCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& InObjectInitilaizer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CL|Character")
	TObjectPtr<UCLPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CL|Character")
	TObjectPtr<UCLCameraComponent> CameraComponent;
};