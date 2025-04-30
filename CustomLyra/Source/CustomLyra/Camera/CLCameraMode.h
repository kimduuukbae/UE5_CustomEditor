#pragma once

#include "CLCameraMode.generated.h"

UCLASS(Abstract)
class UCLCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UCLCameraMode(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

public:

	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendAlpha = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendWeight = 1.0f;
};

UCLASS()
class UCLCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UCLCameraModeStack(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	TObjectPtr<UCLCameraMode> GetCameraModeInstance(const TSubclassOf<UCLCameraMode>& InCameraModeClass);
	void PushCameraMode(const TSubclassOf<UCLCameraMode>& InCameraModeClass);
private:
	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeStack;
};