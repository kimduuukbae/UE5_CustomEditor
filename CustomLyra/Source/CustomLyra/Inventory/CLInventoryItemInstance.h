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

	UPROPERTY()
	TSubclassOf<UCLInventoryItemDefinition> ItemDefinitions;
};