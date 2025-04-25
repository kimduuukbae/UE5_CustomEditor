#include "CLCameraComponent.h"
#include "CLCameraMode.h"

UCLCameraComponent::UCLCameraComponent(const FObjectInitializer& InObjectInitializer) : 
	Super(InObjectInitializer),
	CameraModeStack(nullptr)
{
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
}

void UCLCameraComponent::UpdateCameraModes()
{
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<UCLCameraMode> cameraMode = DetermineCameraModeDelegate.Execute())
		{

		}
	}
}
