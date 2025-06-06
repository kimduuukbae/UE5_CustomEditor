#pragma once

#include "UObject/Object.h"
#include "CLEquipmentInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UCLEquipmentInstance : public UObject
{
GENERATED_BODY()
public:
	UCLEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
