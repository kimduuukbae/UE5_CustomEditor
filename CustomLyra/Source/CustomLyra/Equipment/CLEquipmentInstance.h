#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"
#include "CLEquipmentInstance.generated.h"

struct FCLEquipmentActorToSpawn;

UCLASS(BlueprintType, Blueprintable)
class UCLEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UCLEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	virtual void OnEquipped();
	virtual void OnUnequipped();

	void SpawnEquipmentActors(const TArray<FCLEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
