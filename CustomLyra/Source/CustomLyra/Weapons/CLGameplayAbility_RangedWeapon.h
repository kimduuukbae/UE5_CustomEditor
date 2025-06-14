#pragma once

#include "CoreMinimal.h"
#include "CustomLyra/Equipment/CLGameplayAbility_FromEquipment.h"
#include "CLGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class UCLGameplayAbility_RangedWeapon : public UCLGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	UCLGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
};