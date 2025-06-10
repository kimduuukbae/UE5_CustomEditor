#include "CLInventoryManagerComponent.h"
#include "CLInventoryItemDefinition.h"
#include "CLInventoryItemInstance.h"

UCLInventoryItemInstance* FCLInventoryList::AddEntry(TSubclassOf<UCLInventoryItemDefinition> ItemDefinition)
{
	UCLInventoryItemInstance* result = nullptr;
	AActor* owningActor = OwnerComponent->GetOwner();

	FCLInventoryEntry& newEntry = Entries.AddDefaulted_GetRef();
	newEntry.Instance = NewObject<UCLInventoryItemInstance>(owningActor);
	newEntry.Instance->ItemDefinition = ItemDefinition;

	result = newEntry.Instance;
	return result;
}

UCLInventoryManagerComponent::UCLInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}, InventoryList(this)
{
}

UCLInventoryItemInstance* UCLInventoryManagerComponent::AddItemDefinition(TSubclassOf<UCLInventoryItemDefinition> ItemDefinition)
{
	UCLInventoryItemInstance* result = nullptr;
	if (IsValid(ItemDefinition) == true)
	{
		result = InventoryList.AddEntry(ItemDefinition);
	}

	return result;
}