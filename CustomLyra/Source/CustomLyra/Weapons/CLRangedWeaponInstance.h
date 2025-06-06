#pragma once

#include "CLWeaponInstance.h"
#include "CLRangedWeaponInstance.generated.h"

UCLASS()
class UCLRangedWeaponInstance : public UCLWeaponInstance
{
	GENERATED_BODY()
public:
	UCLRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};