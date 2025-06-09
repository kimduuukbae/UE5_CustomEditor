#pragma once

#include "Components/ControllerComponent.h"
#include "CLQuickBarComponent.generated.h"

class UCLInventoryItemInstance;
class UCLEquipmentInstance;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UCLQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	UCLQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<UCLInventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UCLEquipmentInstance> EquippedItem;
};