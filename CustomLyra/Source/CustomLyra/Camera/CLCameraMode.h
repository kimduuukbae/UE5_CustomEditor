#pragma once

#include "CLCameraMode.generated.h"

UCLASS()
class UCLCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UCLCameraModeStack(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeStack;
};