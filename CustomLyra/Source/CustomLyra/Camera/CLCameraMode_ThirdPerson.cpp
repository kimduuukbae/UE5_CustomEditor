#include "CLCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

UCLCameraMode_ThirdPerson::UCLCameraMode_ThirdPerson(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

void UCLCameraMode_ThirdPerson::UpdateView(float InDeltaTime)
{
	FVector pivotLocation = GetPivotLocation();
	FRotator pivotRotation = GetPivotRotation();

	pivotRotation.Pitch = FMath::ClampAngle(pivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = pivotLocation;
	View.Rotation = pivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	if (IsValid(TargetOffsetCurve) == true)
	{
		FVector targetOffset = TargetOffsetCurve->GetVectorValue(pivotRotation.Pitch);
		View.Location = pivotLocation + pivotRotation.RotateVector(targetOffset);
	}
}
