#pragma once

#include "CLInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "CLInventoryFragment_EquippableItem.generated.h"

class UCLEquipmentDefinition;

UCLASS()
class UCLInventoryFragment_EquippableItem : public UCLInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "CL")
	TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition;
};
