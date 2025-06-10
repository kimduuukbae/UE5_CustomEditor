#pragma once

#include "Components/ControllerComponent.h"
#include "CLQuickBarComponent.generated.h"

class UCLInventoryItemInstance;
class UCLEquipmentInstance;
class UCLEquipmentManagerComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UCLQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	UCLQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UCLInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(int32 NewIndex);

	UCLEquipmentManagerComponent* FindEquipmentManager();
	void EquipItemInSlot();
	void UnequipItemInSlot();

	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<UCLInventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UCLEquipmentInstance> EquippedItem;
};