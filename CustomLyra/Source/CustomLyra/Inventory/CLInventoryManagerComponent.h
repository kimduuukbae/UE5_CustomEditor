#pragma once

#include "Components/ActorComponent.h"
#include "CLInventoryManagerComponent.generated.h"

class UCLInventoryItemInstance;

USTRUCT(BlueprintType)
struct FCLInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCLInventoryItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FCLInventoryList
{
	GENERATED_BODY()

	FCLInventoryList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent( InOwnerComponent ) {}

	UPROPERTY()
	TArray<FCLInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

// UControllerComponent를 상속받고 있다고 생각하는게 좋다.
// PlayerController에 붙을 애라서
UCLASS(BlueprintType)
class UCLInventoryManagerComponent : public UActorComponent 
{
	GENERATED_BODY()
public:
	UCLInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FCLInventoryList InventoryList;
};