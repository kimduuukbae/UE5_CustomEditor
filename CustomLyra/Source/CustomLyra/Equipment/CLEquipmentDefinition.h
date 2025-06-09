#pragma once

#include "CLEquipmentDefinition.generated.h"

class UCLEquipmentInstance;

USTRUCT()
struct FCLEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
};

UCLASS(BlueprintType, Blueprintable)
class UCLEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UCLEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<UCLEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FCLEquipmentActorToSpawn> ActorsToSpawn;
};