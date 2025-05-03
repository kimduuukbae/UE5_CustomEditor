#pragma once

#include "CLCameraMode.h"
#include "CLCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract, Blueprintable)
class UCLCameraMode_ThirdPerson : public UCLCameraMode
{
	GENERATED_BODY()

public:
	UCLCameraMode_ThirdPerson(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};