#include "CLWeaponInstance.h"

UCLWeaponInstance::UCLWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

TSubclassOf<UAnimInstance> UCLWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTag) const
{
	const FCLAnimLayerSelectionSet& setToQuery = bEquipped == true ? EquippedAnimSet : UnequippedAnimSet;
	return setToQuery.SelectBestLayer(CosmeticTag);
}
