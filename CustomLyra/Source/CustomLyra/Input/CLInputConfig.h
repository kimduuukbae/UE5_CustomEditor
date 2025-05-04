#pragma once

#include "Containers/Array.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CLInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FCLInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType)
class UCLInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UCLInputConfig(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	TObjectPtr<const UInputAction> FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	TObjectPtr<const UInputAction> FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FCLInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FCLInputAction> AbilityInputActions;
};