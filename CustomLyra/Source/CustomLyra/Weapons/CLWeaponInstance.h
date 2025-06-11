#pragma once

#include "CustomLyra/Equipment/CLEquipmentInstance.h"
#include "CustomLyra/Cosmetics/CLCosmeticAnimationTypes.h"
#include "CLWeaponInstance.generated.h"

UCLASS()
class UCLWeaponInstance : public UCLEquipmentInstance
{
	GENERATED_BODY()
public:
	UCLWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTag) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLAnimLayerSelectionSet UnequippedAnimSet;
};