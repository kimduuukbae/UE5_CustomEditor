#pragma once

#include "Camera/CameraComponent.h"
#include "CLCameraComponent.generated.h"

class UCLCameraMode;

UCLASS()
class UCLCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UCLCameraComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY()
	TObjectPtr<UCLCameraModeStack> CameraModeStack;
};