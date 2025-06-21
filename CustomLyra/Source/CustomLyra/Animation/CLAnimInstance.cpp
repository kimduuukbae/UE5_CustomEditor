#include "CLAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "CustomLyra/AbilitySystem/CLAbilitySystemComponent.h"

UCLAnimInstance::UCLAnimInstance(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* owningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(owningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UCLAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* InASC)
{
	GameplayTagPropertyMap.Initialize(this, InASC);
}
