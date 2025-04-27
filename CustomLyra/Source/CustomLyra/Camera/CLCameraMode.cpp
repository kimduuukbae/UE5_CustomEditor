#include "CLCameraMode.h"

UCLCameraMode::UCLCameraMode(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

UCLCameraModeStack::UCLCameraModeStack(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

TObjectPtr<UCLCameraMode> UCLCameraModeStack::GetCameraModeInstance(TSubclassOf<UCLCameraMode>& InCameraModeClass)
{
    if (IsValid(InCameraModeClass) == false)
    {
        return;
    }

    for (TObjectPtr<UCLCameraMode> cameraMode : CameraModeInstances)
    {
        if (IsValid(cameraMode) == true && cameraMode->GetClass() == InCameraModeClass)
        {
            return cameraMode;
        }
    }

    TObjectPtr<UCLCameraMode> newCameraMode = NewObject<UCLCameraMode>(GetOuter(), InCameraModeClass, NAME_None, RF_NoFlags);
    CameraModeInstances.Add(newCameraMode);

    return newCameraMode;
}

void UCLCameraModeStack::PushCameraMode(TSubclassOf<UCLCameraMode>& InCameraModeClass)
{
    if (IsValid(InCameraModeClass) == false)
    {
        return;
    }

    TObjectPtr<UCLCameraMode> cameraMode = GetCameraModeInstance(InCameraModeClass);
}
