#include "CLCameraComponent.h"
#include "CLCameraMode.h"

UCLCameraComponent::UCLCameraComponent(const FObjectInitializer& InObjectInitializer) : 
	Super(InObjectInitializer),
	CameraModeStack(nullptr)
{
}

TObjectPtr<AActor> UCLCameraComponent::GetTargetActor() const
{
	return GetOwner();
}

void UCLCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (IsValid(CameraModeStack) == false)
	{
		CameraModeStack = NewObject<UCLCameraModeStack>(this);
	}
}

void UCLCameraComponent::GetCameraView(float InDeltaTime, FMinimalViewInfo& DesiredView)
{
	UpdateCameraModes();

	FCLCameraModeView cameraModeView;
	CameraModeStack->EvaluateStack(InDeltaTime, cameraModeView);
}

void UCLCameraComponent::UpdateCameraModes()
{
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<UCLCameraMode> cameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(cameraMode);
		}
	}
}
