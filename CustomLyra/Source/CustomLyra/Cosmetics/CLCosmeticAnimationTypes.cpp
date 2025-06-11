#include "CLCosmeticAnimationTypes.h"

USkeletalMesh* FCLAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTag) const
{
	for (const FCLAnimBodyStyleSelectionEntry& rule : MeshRules)
	{
		if (IsValid(rule.Mesh) == true && CosmeticTag.HasAll(rule.RequiredTags) == true)
		{
			return rule.Mesh;
		}
	}

	return DefaultMesh;
}

TSubclassOf<UAnimInstance> FCLAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FCLAnimLayerSelectionEntry& rule : LayerRules)
	{
		if (rule.Layer != nullptr && CosmeticTags.HasAll(rule.RequiredTags) == true)
		{
			return rule.Layer;
		}
	}

	return DefaultLayer;
}
