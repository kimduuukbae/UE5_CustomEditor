#include "CLGameplayAbility_FromEquipment.h"
#include "CLEquipmentInstance.h"

UCLGameplayAbility_FromEquipment::UCLGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

UCLEquipmentInstance* UCLGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
    if (FGameplayAbilitySpec* spec = UGameplayAbility::GetCurrentAbilitySpec())
    {
        return Cast<UCLEquipmentInstance>(spec->SourceObject.Get());
    }
    return nullptr;
}
