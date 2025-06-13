#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "CLAbilitySet.generated.h"

class UCLGameplayAbility;
class UCLAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

USTRUCT(BlueprintType)
struct FCLAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCLGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FCLAbilitySet_GrantedHandles
{
	GENERATED_BODY()
	
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UCLAbilitySystemComponent* InASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

UCLASS(BlueprintType)
class UCLAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCLAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UCLAbilitySystemComponent* InASC, FCLAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FCLAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};