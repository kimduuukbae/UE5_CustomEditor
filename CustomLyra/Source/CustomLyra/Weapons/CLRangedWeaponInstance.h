#pragma once

#include "CLWeaponInstance.h"
#include "CLRangedWeaponInstance.generated.h"

UCLASS()
class UCLRangedWeaponInstance : public UCLWeaponInstance
{
	GENERATED_BODY()
public:
	UCLRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};