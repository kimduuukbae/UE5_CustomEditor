#include "CLPawnComponent_CharacterParts.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"

FCLCharacterPartHandle FCLCharacterPartList::AddEntry(FCLCharacterPart NewPart)
{
	FCLCharacterPartHandle handle;
	handle.PartHandle = PartHandleCounter++;

	if (IsValid(OwnerComponent) == true && IsValid(OwnerComponent->GetOwner()) == true && OwnerComponent->GetOwner()->HasAuthority())
	{
		FCLAppliedCharacterPartEntry& newEntry = Entries.AddDefaulted_GetRef();
		newEntry.Part = NewPart;
		newEntry.PartHandle = handle.PartHandle;

		if (SpawnActorForEntry(newEntry) == true)
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return handle;
}

void FCLCharacterPartList::RemoveEntry(FCLCharacterPartHandle Handle)
{
	for (auto entryIt = Entries.CreateIterator(); entryIt; ++entryIt)
	{
		FCLAppliedCharacterPartEntry& entry = *entryIt;

		if (entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(entry);
		}
	}
}

bool FCLCharacterPartList::SpawnActorForEntry(FCLAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	if (IsValid(Entry.Part.PartClass) == true)
	{
		TObjectPtr<UWorld> world = OwnerComponent->GetWorld();

		if (TObjectPtr<USceneComponent> componentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			const FTransform spawnTransform = componentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			UChildActorComponent* partComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			// SetupAttachment는 AttachParent를 셋만 해주고
			// 실제로 RegisterComponent가 불렸을때 OnRegister가 불리면서 USceneComponent::AttachToComponent가 일어난다.
			partComponent->SetupAttachment(componentToAttachTo, Entry.Part.SocketName);
			partComponent->SetChildActorClass(Entry.Part.PartClass);
			partComponent->RegisterComponent();

			if (TObjectPtr<AActor> spawnedActor = partComponent->GetChildActor())
			{
				if (TObjectPtr<USceneComponent> spawnedSceneComponent = spawnedActor->GetRootComponent())
				{
					spawnedSceneComponent->AddTickPrerequisiteComponent(componentToAttachTo);
				}
			}

			Entry.SpawnedComponent = partComponent;
			bCreatedAnyActor = true;
		}
	}

	return bCreatedAnyActor;
}

FGameplayTagContainer FCLCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer result;

	for (const FCLAppliedCharacterPartEntry& entry : Entries)
	{
		if (IsValid(entry.SpawnedComponent) == true)
		{
			if (IGameplayTagAssetInterface* tagInterface = Cast<IGameplayTagAssetInterface>(entry.SpawnedComponent->GetChildActor()))
			{
				tagInterface->GetOwnedGameplayTags(result);
			}
		}
	}

	return result;
}

void FCLCharacterPartList::DestroyActorForEntry(FCLAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

UCLPawnComponent_CharacterParts::UCLPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) 
	: Super{ObjectInitializer},
	CharacterPartList(this)
{
}

FCLCharacterPartHandle UCLPawnComponent_CharacterParts::AddCharacterPart(const FCLCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void UCLPawnComponent_CharacterParts::RemoveCharacterPart(FCLCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

USkeletalMeshComponent* UCLPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* ownerActor = GetOwner())
	{
		if (ACharacter* character = Cast<ACharacter>(ownerActor))
		{
			if (USkeletalMeshComponent* meshComponent = character->GetMesh())
			{
				return meshComponent;
			}
		}
	}
	return nullptr;
}

USceneComponent* UCLPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* meshComponent = GetParentMeshComponent())
	{
		return meshComponent;
	}
	else if (AActor* ownerComponent = GetOwner())
	{
		return ownerComponent->GetRootComponent();
	}

	return nullptr;
}

void UCLPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	if (USkeletalMeshComponent* meshComponent = GetParentMeshComponent())
	{
		const FGameplayTagContainer mergedTags = GetCombinedTags(FGameplayTag{});
		USkeletalMesh* desiredMesh = BodyMeshes.SelectBestBodyStyle(mergedTags);

		meshComponent->SetSkeletalMesh(desiredMesh, bReinitPose);

		if (UPhysicsAsset* physicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			meshComponent->SetPhysicsAsset(physicsAsset, bReinitPose);
		}
	}
}

FGameplayTagContainer UCLPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	FGameplayTagContainer result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid() == true)
	{
		return result.Filter(FGameplayTagContainer(RequiredPrefix));
	}

	return result;
}
