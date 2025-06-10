#include "CLInventoryItemInstance.h"
#include "CLInventoryItemDefinition.h"

UCLInventoryItemInstance::UCLInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

const UCLInventoryItemFragment* UCLInventoryItemInstance::FindFragmentByClass(TSubclassOf<UCLInventoryItemFragment> FragmentClass) const
{
	if (ItemDefinition != nullptr && FragmentClass != nullptr)
	{
		return GetDefault<UCLInventoryItemDefinition>(ItemDefinition)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
