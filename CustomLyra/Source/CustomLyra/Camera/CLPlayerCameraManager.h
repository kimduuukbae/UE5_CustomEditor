#pragma once

#include "Camera/PlayerCameraManager.h"
#include "CLPlayerCameraManager.generated.h"

#define CL_CAMERA_DEFAULT_FOV 80.0f
#define CL_CAMERA_DEFAULT_PITCH_MIN -89.0f
#define CL_CAMERA_DEFAULT_PITCH_MAX 89.0f

UCLASS()
class ACLPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACLPlayerCameraManager(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
};