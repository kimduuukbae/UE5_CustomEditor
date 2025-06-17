#pragma once

#include "CoreMinimal.h"
#include "CustomLyra/AbilitySystem/Abilities/CLGameplayAbility.h"
#include "CLGameplayAbility_FromEquipment.generated.h"

UCLASS()
class UCLGameplayAbility_FromEquipment : public UCLGameplayAbility
{
	GENERATED_BODY()

public:
	UCLGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UCLEquipmentInstance* GetAssociatedEquipment() const;
};