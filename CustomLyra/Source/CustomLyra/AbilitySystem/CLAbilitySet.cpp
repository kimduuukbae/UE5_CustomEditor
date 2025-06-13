#include "CLAbilitySet.h"
#include "CLAbilitySystemComponent.h"
#include "Abilities/CLGameplayAbility.h"

UCLAbilitySet::UCLAbilitySet(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void FCLAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid() == true)
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FCLAbilitySet_GrantedHandles::TakeFromAbilitySystem(UCLAbilitySystemComponent* InASC)
{
	if (InASC->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& handle : AbilitySpecHandles)
	{
		if (handle.IsValid() == true)
		{
			InASC->ClearAbility(handle);
		}
	}
}

void UCLAbilitySet::GiveToAbilitySystem(UCLAbilitySystemComponent* InASC, FCLAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	// 멀티플레이 기준에 해당 ASC에 Ownership이 있는지 체크
	if (InASC->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (int32 abilityIndex = 0; abilityIndex < GrantedGameplayAbilities.Num(); ++abilityIndex)
	{
		const FCLAbilitySet_GameplayAbility& abilityToGrant = GrantedGameplayAbilities[abilityIndex];
		if (IsValid(abilityToGrant.Ability) == false)
		{
			continue;
		}


		// EGameplayAbilityInstancingPolicy::InstancedPerActor 
		// 어빌리티 자체를 액터별로 다르게 할당시킬땐 인스턴싱을 다 해줘야 하는데
		// 그거에 대한 Policy
		UCLGameplayAbility* abilityCDO = abilityToGrant.Ability->GetDefaultObject<UCLGameplayAbility>();

		FGameplayAbilitySpec abilitySpec(abilityCDO, abilityToGrant.AbilityLevel);
		abilitySpec.SourceObject = SourceObject;
		abilitySpec.DynamicAbilityTags.AddTag(abilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle abilitySpecHandle = InASC->GiveAbility(abilitySpec);
		if (OutGrantedHandles != nullptr)
		{
			OutGrantedHandles->AddAbilitySpecHandle(abilitySpecHandle);
		}
	}
}