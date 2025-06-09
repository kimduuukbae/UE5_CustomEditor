#include "CLEquipmentDefinition.h"
#include "CLEquipmentInstance.h"

UCLEquipmentDefinition::UCLEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
	InstanceType = UCLEquipmentInstance::StaticClass();
}
