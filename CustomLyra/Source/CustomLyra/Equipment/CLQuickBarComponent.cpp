#include "CLQuickBarComponent.h"
#include "CLEquipmentManagerComponent.h"
#include "CLEquipmentDefinition.h"
#include "CLEquipmentInstance.h"
#include "CustomLyra/Inventory/CLInventoryFragment_EquippableItem.h"
#include "CustomLyra/Inventory/CLInventoryItemInstance.h"

UCLQuickBarComponent::UCLQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	Super::BeginPlay();
}

void UCLQuickBarComponent::AddItemToSlot(int32 SlotIndex, UCLInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) == true && Item != nullptr)
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UCLQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) == true && ActiveSlotIndex != NewIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

UCLEquipmentManagerComponent* UCLQuickBarComponent::FindEquipmentManager()
{
	if (AController* controller = Cast<AController>(GetOwner()))
	{
		if (APawn* pawn = controller->GetPawn())
		{
			return pawn->FindComponentByClass<UCLEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void UCLQuickBarComponent::EquipItemInSlot()
{
	if (UCLInventoryItemInstance* slotItem = Slots[ActiveSlotIndex])
	{
		if (const UCLInventoryFragment_EquippableItem* equipInfo = slotItem->FindFragmentByClass<UCLInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UCLEquipmentDefinition> equipDefinition = equipInfo->EquipmentDefinition;
			if (IsValid(equipDefinition) == true)
			{
				if (UCLEquipmentManagerComponent* componentManager = FindEquipmentManager())
				{
					EquippedItem = componentManager->EquipItem(equipDefinition);
					if (EquippedItem != nullptr)
					{
						EquippedItem->Instigator = slotItem;
					}
				}
			}
		}
	}
	if (UCLEquipmentManagerComponent* managerComponent = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			managerComponent->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UCLQuickBarComponent::UnequipItemInSlot()
{
	if (UCLEquipmentManagerComponent* managerComponent = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			managerComponent->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}
