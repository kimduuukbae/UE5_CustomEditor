#include "CLEquipmentManagerComponent.h"
#include "CLEquipmentInstance.h"
#include "CLEquipmentDefinition.h"

UCLEquipmentInstance* FCLEquipmentList::AddEntry(TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition)
{
	UCLEquipmentInstance* result = nullptr;
	
	const UCLEquipmentDefinition* equipmentCDO = GetDefault<UCLEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UCLEquipmentInstance> instanceType = equipmentCDO->InstanceType;
	if (instanceType == nullptr)
	{
		instanceType = UCLEquipmentInstance::StaticClass();
	}

	FCLAppliedEquipmentEntry& newEntry = Entries.AddDefaulted_GetRef();
	newEntry.EquipmentDefinition = EquipmentDefinition;
	newEntry.Instance = NewObject<UCLEquipmentInstance>(OwnerComponent->GetOwner(), instanceType);
	result = newEntry.Instance;

	result->SpawnEquipmentActors(equipmentCDO->ActorsToSpawn);

	return result;
}

void FCLEquipmentList::RemoveEntry(UCLEquipmentInstance* Instance)
{
	for (auto it = Entries.CreateIterator(); it; ++it)
	{
		FCLAppliedEquipmentEntry& entry = *it;
		if (entry.Instance == Instance)
		{
			Instance->DestroyEquipmentActors();
			it.RemoveCurrent();
		}
	}
}

UCLEquipmentManagerComponent::UCLEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipmentList(this)
{
}

void UCLEquipmentManagerComponent::UnequipItem(UCLEquipmentInstance* Instance)
{
	if (Instance != nullptr)
	{
		Instance->OnUnequipped();

		EquipmentList.RemoveEntry(Instance);
	}
}

UCLEquipmentInstance* UCLEquipmentManagerComponent::EquipItem(TSubclassOf<UCLEquipmentDefinition> EquipmentDefinition)
{
	UCLEquipmentInstance* result = nullptr;
	if (EquipmentDefinition != nullptr)
	{
		result = EquipmentList.AddEntry(EquipmentDefinition);
		if (result != nullptr)
		{
			result->OnEquipped();
		}
	}

	return result;
}