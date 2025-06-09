#pragma once

#include "Components/PawnComponent.h"
#include "CLEquipmentManagerComponent.generated.h"

class UCLEquipmentDefinition;
class UCLEquipmentInstance;

USTRUCT(BlueprintType)
struct FCLAppliedEquipmentEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UCLEquipmentInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FCLEquipmentList
{
	GENERATED_BODY()

	FCLEquipmentList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent(InOwnerComponent) {} 

	UPROPERTY()
	TArray<FCLAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent = nullptr;
};

UCLASS(BlueprintType)
class UCLEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UCLEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FCLEquipmentList EquipmentList;
};