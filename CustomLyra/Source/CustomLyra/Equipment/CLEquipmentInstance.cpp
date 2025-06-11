#include "CLEquipmentInstance.h"
#include "CLEquipmentDefinition.h"
#include "GameFramework/Character.h"

UCLEquipmentInstance::UCLEquipmentInstance(const FObjectInitializer& ObjectInitializer)
{
}

APawn* UCLEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCLEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* result = nullptr;
	if (UClass* actualPawnType = PawnType)
	{
		if (GetOuter()->IsA(actualPawnType) == true)
		{
			result = Cast<APawn>(GetOuter());
		}
	}

	return result;
}

void UCLEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UCLEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UCLEquipmentInstance::SpawnEquipmentActors(const TArray<FCLEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* owningPawn = GetPawn())
	{
		USceneComponent* attachTarget = owningPawn->GetRootComponent();
		if (ACharacter* character = Cast<ACharacter>(owningPawn))
		{
			attachTarget = character->GetMesh();
		}

		for (const FCLEquipmentActorToSpawn& spawnInfo : ActorsToSpawn)
		{
			AActor* newActor = GetWorld()->SpawnActorDeferred<AActor>(spawnInfo.ActorToSpawn, FTransform::Identity, owningPawn);
			newActor->FinishSpawning(FTransform::Identity, true);

			newActor->SetActorRelativeTransform(spawnInfo.AttachTransform);
			newActor->AttachToComponent(attachTarget, FAttachmentTransformRules::KeepRelativeTransform, spawnInfo.AttachSocket);

			SpawnedActors.Add(newActor);
		}
	}
}

void UCLEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* actor : SpawnedActors)
	{
		if (IsValid(actor) == true)
		{
			actor->Destroy();
		}
	}
}
