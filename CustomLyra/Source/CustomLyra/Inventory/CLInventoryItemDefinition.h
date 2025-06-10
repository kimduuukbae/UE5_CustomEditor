#pragma once

#include "CLInventoryItemDefinition.generated.h"

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UCLInventoryItemFragment : public UObject
{
	GENERATED_BODY()
};

// 아이템이 어떠한 정의를 가지고 있는가 표현하는 데이터
UCLASS(Blueprintable)
class UCLInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UCLInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UCLInventoryItemFragment* FindFragmentByClass(TSubclassOf<UCLInventoryItemFragment> FragmentClass);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Display")
	TArray<TObjectPtr<UCLInventoryItemFragment>> Fragments;
};