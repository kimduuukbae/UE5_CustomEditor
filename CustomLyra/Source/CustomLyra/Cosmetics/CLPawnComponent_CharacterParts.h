#pragma once

#include "Components/PawnComponent.h"
#include "CLCosmeticAnimationTypes.h"
#include "CLCharacterPartTypes.h"
#include "CLPawnComponent_CharacterParts.generated.h"

USTRUCT()
struct FCLAppliedCharacterPartEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FCLCharacterPart Part;

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// Actor와 Actor 끼리 연결해주는 Component
	// AActor도 이렇게 붙이고 있는데 AActor의 ParentComponent를 보시라.
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FCLCharacterPartList
{
	GENERATED_BODY()

	FCLCharacterPartList() : OwnerComponent(nullptr){}
	FCLCharacterPartList(UCLPawnComponent_CharacterParts* InOwnerComponent) : OwnerComponent( InOwnerComponent ) {}

	UPROPERTY()
	TArray<FCLAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UCLPawnComponent_CharacterParts> OwnerComponent;

	int32 PartHandleCounter = 0;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UCLPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UCLPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FCLCharacterPartList CharacterPartList;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FCLAnimBodyStyleSelectionSet BodyMeshes;
};