#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"
#include "CLEquipmentInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UCLEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UCLEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
