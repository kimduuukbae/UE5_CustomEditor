#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "CLInventoryItemInstance.generated.h"

class UCLInventoryItemDefinition;

UCLASS(BlueprintType)
class UCLInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UCLInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UCLInventoryItemFragment* FindFragmentByClass(TSubclassOf<UCLInventoryItemFragment> FragmentClass) const;
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	UPROPERTY()
	TSubclassOf<UCLInventoryItemDefinition> ItemDefinition;
};