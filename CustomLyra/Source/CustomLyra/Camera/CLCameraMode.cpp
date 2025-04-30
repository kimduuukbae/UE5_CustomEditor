#include "CLCameraMode.h"

UCLCameraMode::UCLCameraMode(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

UCLCameraModeStack::UCLCameraModeStack(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

TObjectPtr<UCLCameraMode> UCLCameraModeStack::GetCameraModeInstance(const TSubclassOf<UCLCameraMode>& InCameraModeClass)
{
    if (IsValid(InCameraModeClass) == false)
    {
        return nullptr;
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

void UCLCameraModeStack::PushCameraMode(const TSubclassOf<UCLCameraMode>& InCameraModeClass)
{
    if (IsValid(InCameraModeClass) == false)
    {
        return;
    }

    TObjectPtr<UCLCameraMode> cameraMode = GetCameraModeInstance(InCameraModeClass);

    int32 stackSize = CameraModeStack.Num();
    if (stackSize > 0 && CameraModeStack[0] == cameraMode)
    {
        return;
    }

    int32 existingStackIndex = INDEX_NONE;
    float existingStackContribution = 1.0f;

    for (int32 stackIndex = 0; stackIndex < stackSize; ++stackIndex)
    {
        if (CameraModeStack[stackIndex] == cameraMode)
        {
            existingStackIndex = stackIndex;
            existingStackContribution *= cameraMode->BlendWeight;
        }
        else
        {
            existingStackContribution *= (1.0f - CameraModeStack[stackIndex]->BlendWeight);
        }
    }

    if (existingStackIndex != INDEX_NONE)
    {
        CameraModeStack.RemoveAt(existingStackIndex);
        --stackSize;
    }

    bool bShouldBlend = cameraMode->BlendTime > 0.0f && stackSize > 0;
    float blendWeight = bShouldBlend ? existingStackContribution : 1.0f;
    cameraMode->BlendWeight = blendWeight;

    CameraModeStack.Insert(cameraMode, 0);
    CameraModeStack.Last()->BlendWeight = 1.0f;
}
