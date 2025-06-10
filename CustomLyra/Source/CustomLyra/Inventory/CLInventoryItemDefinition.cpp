#include "CLInventoryItemDefinition.h"

UCLInventoryItemDefinition::UCLInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

const UCLInventoryItemFragment* UCLInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UCLInventoryItemFragment> FragmentClass)
{
	if (FragmentClass != nullptr)
	{
		for (UCLInventoryItemFragment* fragment : Fragments) 
		{
			if (fragment != nullptr && fragment->IsA(FragmentClass))
			{
				return fragment;
			}
		}
	}

	return nullptr;
}
