#pragma once

#include "Camera/CameraComponent.h"
#include "CLCameraComponent.generated.h"

class UCLCameraMode;
class UCLCameraModeStack;
template <typename T> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UCLCameraMode>, FCLCameraModeDelegate);

UCLASS()
class UCLCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UCLCameraComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	static TObjectPtr<UCLCameraComponent> FindCameraComponent(TObjectPtr<const AActor> InActor) { return IsValid(InActor) == true ? InActor->FindComponentByClass<UCLCameraComponent>() : nullptr; }
	
	TObjectPtr<AActor> GetTargetActor() const;

	virtual void OnRegister() override;
	virtual void GetCameraView(float InDeltaTime, FMinimalViewInfo& DesiredView) override;
	void UpdateCameraModes();
	
	FCLCameraModeDelegate DetermineCameraModeDelegate;

private:
	UPROPERTY()
	TObjectPtr<UCLCameraModeStack> CameraModeStack;
};