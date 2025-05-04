#include "CLInputConfig.h"

UCLInputConfig::UCLInputConfig(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

TObjectPtr<const UInputAction> UCLInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCLInputAction& action : NativeInputActions)
	{
		if (action.InputAction != nullptr && action.InputTag == InputTag)
		{
			return action.InputAction;
		}
	}

	return nullptr;
}

TObjectPtr<const UInputAction> UCLInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	return TObjectPtr<const UInputAction>();
}
