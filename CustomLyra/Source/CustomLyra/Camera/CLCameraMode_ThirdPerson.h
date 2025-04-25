#pragma once

#include "CLCameraMode.h"
#include "CLCameraMode_ThirdPerson.generated.h"

UCLASS(Abstract, Blueprintable)
class UCLCameraMode_ThirdPerson : public UCLCameraMode
{
	GENERATED_BODY()

public:
	UCLCameraMode_ThirdPerson(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
};