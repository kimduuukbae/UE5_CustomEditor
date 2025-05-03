#include "CLCameraMode.h"
#include "CLPlayerCameraManager.h"
#include "CLCameraComponent.h"


#pragma region FCLCameraModeView

FCLCameraModeView::FCLCameraModeView() :
    FieldOfView(CL_CAMERA_DEFAULT_FOV)
{

}

void FCLCameraModeView::Blend(const FCLCameraModeView& InView, float Weight)
{
    if (Weight <= 0.0f)
    {
        return;
    }
    else if (Weight >= 1.0f)
    {
        // 1.0f 보다 커지면, 항상 내가 최신으로 지정되어야 한다.
        // 즉 이 전까지의 블렌딩은 무효화 시켜야함
        *this = InView;
        return;
    }

    Location = FMath::Lerp(Location, InView.Location, Weight);
    FRotator deltaRotation = (InView.Rotation - Rotation).GetNormalized();
    Rotation = Rotation + (Weight * deltaRotation);

    FRotator deltaControlRotation = (InView.ControlRotation - ControlRotation).GetNormalized();
    ControlRotation = ControlRotation + (Weight * deltaControlRotation);

    FieldOfView = FMath::Lerp(FieldOfView, InView.FieldOfView, Weight);
}

#pragma endregion


UCLCameraMode::UCLCameraMode(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer),
    FieldOfView(CL_CAMERA_DEFAULT_FOV),
    ViewPitchMin(CL_CAMERA_DEFAULT_PITCH_MIN),
    ViewPitchMax(CL_CAMERA_DEFAULT_PITCH_MAX),
    BlendFunction(ECLCameraModeBlendFunction::EaseOut),
    BlendExponent(4.0f)
{
}

void UCLCameraMode::UpdateCameraMode(float InDeltaTime)
{
    UpdateView(InDeltaTime);
    UpdateBlending(InDeltaTime);
}

TObjectPtr<UCLCameraComponent> UCLCameraMode::GetCameraComponent() const
{
    return CastChecked<UCLCameraComponent>(GetOuter());
}

TObjectPtr<AActor> UCLCameraMode::GetTargetActor() const
{
    TObjectPtr<const UCLCameraComponent> cameraComponent = GetCameraComponent();

    return cameraComponent->GetTargetActor();
}

FVector UCLCameraMode::GetPivotLocation() const
{
    TObjectPtr<const AActor> targetActor = GetTargetActor();

    if (TObjectPtr<const APawn> targetPawn = Cast<APawn>(targetActor))
    {
        return targetPawn->GetPawnViewLocation();
    }

    return targetActor->GetActorLocation();
}

FRotator UCLCameraMode::GetPivotRotation() const 
{
    TObjectPtr<const AActor> targetActor = GetTargetActor();

    if (TObjectPtr<const APawn> pawn = Cast<APawn>(targetActor))
    {
        return pawn->GetViewRotation();
    }

    return targetActor->GetActorRotation();
}

void UCLCameraMode::UpdateView(float InDeltaTime)
{
    FVector pivotLocation = GetPivotLocation();
    FRotator pivotRotation = GetPivotRotation();

    pivotRotation.Pitch = FMath::ClampAngle(pivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

    View.Location = pivotLocation;
    View.Rotation = pivotRotation;

    View.ControlRotation = View.Rotation;
    View.FieldOfView = FieldOfView;
}

void UCLCameraMode::UpdateBlending(float InDeltaTime)
{
    if (BlendTime > 0.0f)
    {
        BlendAlpha += InDeltaTime / BlendTime;
    }
    else
    {
        BlendAlpha = 1.0f;
    }

    float exponent = BlendExponent > 0.0f ? BlendExponent : 1.0f;
    switch (BlendFunction)
    {
    case ECLCameraModeBlendFunction::Linear:
        BlendWeight = BlendAlpha;
        break;
    case ECLCameraModeBlendFunction::EaseIn:
        BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, exponent);
        break;
    case ECLCameraModeBlendFunction::EaseOut:
        BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, exponent);
        break;
    case ECLCameraModeBlendFunction::EaseInOut:
        BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, exponent);
        break;
    default:
        break;
    }
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

void UCLCameraModeStack::EvaluateStack(float InDeltaTime, FCLCameraModeView& OutCameraModeView)
{
    UpdateStack(InDeltaTime);
    BlendStack(OutCameraModeView);
}

void UCLCameraModeStack::UpdateStack(float InDeltaTime)
{
    int32 stackSize = CameraModeStack.Num();
    if (stackSize <= 0)
    {
        return;
    }

    int32 removeCount = 0;
    int32 removeIndex = INDEX_NONE;
    for (int32 stackIndex = 0; stackIndex < stackSize; ++stackIndex)
    {
        TObjectPtr<UCLCameraMode> cameraMode = CameraModeStack[stackIndex];

        cameraMode->UpdateCameraMode(InDeltaTime);

        if (cameraMode->BlendWeight >= 1.0f)
        {
            removeIndex = stackIndex + 1;
            removeCount = stackSize - removeIndex;
            break;
        }
    }

    if (removeCount > 0)
    {
        CameraModeStack.RemoveAt(removeIndex, removeCount);
    }
}

void UCLCameraModeStack::BlendStack(FCLCameraModeView& OutCameraModeView) const
{
    const int32 stackSize = CameraModeStack.Num();
    if (stackSize <= 0)
    {
        return;
    }

    TObjectPtr<const UCLCameraMode> cameraMode = CameraModeStack[stackSize - 1];

    OutCameraModeView = cameraMode->View;


    // 맨 뒤부터 시작한다. 맨 앞에 있는것이 가장 최근에 들어왔을거고
    // 맨 뒤에 있는 것이 deltaTime이 가장 크게 누적됐을것이기 때문에 그녀석을 기준으로 블렌딩을 시켜준다.
    for (int32 stackIndex = stackSize - 2; stackIndex >= 0; --stackIndex)
    {
        cameraMode = CameraModeStack[stackIndex];
        OutCameraModeView.Blend(cameraMode->View, cameraMode->BlendWeight);
    }
}
