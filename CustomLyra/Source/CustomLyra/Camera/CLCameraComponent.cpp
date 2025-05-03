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

	if (TObjectPtr<APawn> pawn = Cast<APawn>(GetTargetActor()))
	{
		if (TObjectPtr<APlayerController> playerController = pawn->GetController<APlayerController>())
		{
			// RootComponent->SetWorldRotation(GetControlRotation());
			// 즉, possess 하고 있는 character의 rootComponent의 Rotation을 변경한다.
			playerController->SetControlRotation(cameraModeView.ControlRotation);
		}
	}

	SetWorldLocationAndRotation(cameraModeView.Location, cameraModeView.Rotation);

	FieldOfView = cameraModeView.FieldOfView;

	DesiredView.Location = cameraModeView.Location;
	DesiredView.Rotation = cameraModeView.Rotation;
	DesiredView.FOV = cameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
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
