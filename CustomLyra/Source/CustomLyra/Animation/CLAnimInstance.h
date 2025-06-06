#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CLAnimInstance.generated.h"

UCLASS()
class UCLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCLAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};