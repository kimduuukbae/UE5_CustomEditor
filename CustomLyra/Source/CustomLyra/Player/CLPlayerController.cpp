#include "CLPlayerController.h"
#include "CustomLyra/Camera/CLPlayerCameraManager.h"

ACLPlayerController::ACLPlayerController(const FObjectInitializer& InInitializer) : Super{InInitializer}
{
	PlayerCameraManagerClass = ACLPlayerCameraManager::StaticClass();
}
