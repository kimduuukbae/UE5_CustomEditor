#pragma once

#include "Components/PawnComponent.h"
#include "CustomLyra/AbilitySystem/CLAbilitySet.h"
#include "CustomLyra/AbilitySystem/CLAbilitySystemComponent.h"
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

	UPROPERTY()
	FCLAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FCLEquipmentList
{
	GENERATED_BODY()

	FCLEquipmentList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent(InOwnerComponent) {} 

	UCLEquipmentInstance* AddEntry(TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UCLEquipmentInstance* Instance);
	UCLAbilitySystemComponent* GetAbilitySystemComponent();

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

	void UnequipItem(UCLEquipmentInstance* Instance);
	UCLEquipmentInstance* EquipItem(TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable)
	TArray<UCLEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UCLEquipmentInstance> InstanceType) const;

	UPROPERTY()
	FCLEquipmentList EquipmentList;
};