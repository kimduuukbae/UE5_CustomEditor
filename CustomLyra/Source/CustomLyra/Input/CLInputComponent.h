#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "CLInputConfig.h"
#include "CLInputComponent.generated.h"

UCLASS()
class UCLInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UCLInputComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	template <typename UserClass, typename FuncType>
	void BindNativeAction(TObjectPtr<const UCLInputConfig> InputConfig, const FGameplayTag& InputTag, ETriggerEvent InTriggerEvent, UserClass* InObject, FuncType InFuncType);

	template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(TObjectPtr<const UCLInputConfig> InputConfig, UserClass* InObject, PressedFuncType InPressedFunc, ReleasedFuncType InReleasedFunc, TArray<uint32>& BindHandles);
};

template<typename UserClass, typename FuncType>
inline void UCLInputComponent::BindNativeAction(TObjectPtr<const UCLInputConfig> InputConfig, const FGameplayTag& InputTag, ETriggerEvent InTriggerEvent, UserClass* InObject, FuncType InFuncType)
{
	if (TObjectPtr<const UInputAction> IA = InputConfig->FindNativeInputActionForTag(InputTag, true))
	{
		BindAction(IA, InTriggerEvent, InObject, InFuncType);
	}
}

template<typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void UCLInputComponent::BindAbilityActions(TObjectPtr<const UCLInputConfig> InputConfig, UserClass* InObject, PressedFuncType InPressedFunc, ReleasedFuncType InReleasedFunc, TArray<uint32>& BindHandles)
{
	for (const FCLInputAction& inputAction : InputConfig->AbilityInputActions)
	{
		if (inputAction.InputAction != nullptr && inputAction.InputTag.IsValid() == true)
		{
			if (InPressedFunc)
			{
				BindHandles.Add(BindAction(inputAction.InputAction, ETriggerEvent::Triggered, InObject, InPressedFunc, inputAction.InputTag).GetHandle());
			}

			if (InReleasedFunc)
			{
				BindHandles.Add(BindAction(inputAction.InputAction, ETriggerEvent::Completed, InObject, InReleasedFunc, inputAction.InputTag).GetHandle());
			}
		}
	}
}
