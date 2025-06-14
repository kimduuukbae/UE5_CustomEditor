#include "CLAbilitySystemComponent.h"
#include "Abilities/CLGameplayAbility.h"
UCLAbilitySystemComponent::UCLAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		for (const FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
		{
			if (IsValid(abilitySpec.Ability) == true && abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(abilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(abilitySpec.Handle);
			}
		}
	}
}

void UCLAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		for (const FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
		{
			if (IsValid(abilitySpec.Ability) == true && abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(abilitySpec.Handle);
				InputHeldSpecHandles.Remove(abilitySpec.Handle);
			}
		}
	}
}

void UCLAbilitySystemComponent::ProcessAbilityInput(const float DeltaTime, const bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> abilitiesToActivate;

	for (const FGameplayAbilitySpecHandle& specHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle))
		{
			if (IsValid(abilitySpec->Ability) == true && abilitySpec->IsActive() == false)
			{
				const UCLGameplayAbility* abilityCDO = Cast<UCLGameplayAbility>(abilitySpec->Ability);

				if (abilityCDO->ActivationPolicy == ECLAbilityActivationPolicy::WhileInputActive)
				{
					abilitiesToActivate.AddUnique(abilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& specHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle))
		{
			if (IsValid(abilitySpec->Ability) == true)
			{
				if (abilitySpec->IsActive() == true)
				{
					AbilitySpecInputPressed(*abilitySpec);
				}
				else
				{
					const UCLGameplayAbility* abilityCDO = Cast<UCLGameplayAbility>(abilitySpec->Ability);

					if (abilityCDO->ActivationPolicy == ECLAbilityActivationPolicy::OnInputTriggered)
					{
						abilitiesToActivate.AddUnique(abilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& abilitySpecHandle : abilitiesToActivate)
	{
		TryActivateAbility(abilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& specHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle))
		{
			if (abilitySpec->Ability)
			{
				abilitySpec->InputPressed = false;
				if (abilitySpec->IsActive() == true)
				{
					AbilitySpecInputReleased(*abilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
