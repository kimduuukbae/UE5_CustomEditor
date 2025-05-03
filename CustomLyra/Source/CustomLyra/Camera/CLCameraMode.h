#pragma once

#include "CLCameraMode.generated.h"

class UCLCameraComponent;

UENUM(BlueprintType)
enum class ECLCameraModeBlendFunction : uint8
{
	Linear = 0,
	/* EaseIn, EaseOut은 Exponent 값에 의해 조절됨 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

struct FCLCameraModeView
{
	FCLCameraModeView();

	void Blend(const FCLCameraModeView& InView, float Weight);

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FRotator ControlRotation = FRotator::ZeroRotator;	// 카메라가 움직이면 같이 움직일 ControlRotation
	// SpringArm과 CameraComponent의 의 bUsePawnControlRotation 이다.
	float FieldOfView = FLT_MAX;
};

UCLASS(Abstract)
class UCLCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UCLCameraMode(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	void UpdateCameraMode(float InDeltaTime);

protected:
	TObjectPtr<UCLCameraComponent> GetCameraComponent() const;
	TObjectPtr<AActor> GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	virtual void UpdateView(float InDeltaTime);
	void UpdateBlending(float InDeltaTime);

public:

	FCLCameraModeView View;

	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendAlpha = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Blending")
	float BlendWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	ECLCameraModeBlendFunction BlendFunction;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = 5.0, UIMax = 170.0, ClampMin = 5.0, ClampMax = 170.0))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = -89.9, UIMax = 89.9, ClampMin = -89.9, ClampMax = 89.9))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = -89.9, UIMax = 89.9, ClampMin = -89.9, ClampMax = 89.9))
	float ViewPitchMax;
};

UCLASS()
class UCLCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UCLCameraModeStack(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	TObjectPtr<UCLCameraMode> GetCameraModeInstance(const TSubclassOf<UCLCameraMode>& InCameraModeClass);
	void PushCameraMode(const TSubclassOf<UCLCameraMode>& InCameraModeClass);
	void EvaluateStack(float InDeltaTime, FCLCameraModeView& OutCameraModeView);

private:
	void UpdateStack(float InDeltaTime);
	void BlendStack(FCLCameraModeView& OutCameraModeView) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UCLCameraMode>> CameraModeStack;
};