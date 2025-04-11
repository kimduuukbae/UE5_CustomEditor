#pragma once

#include "Camera/PlayerCameraManager.h"
#include "CLPlayerCameraManager.generated.h"

UCLASS()
class ACLPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACLPlayerCameraManager(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
};