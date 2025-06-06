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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLAnimLayerSelectionSet UnequippedAnimSet;
};