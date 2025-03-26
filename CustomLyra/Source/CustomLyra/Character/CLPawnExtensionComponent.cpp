#include "CLPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"

UCLPawnExtensionComponent::UCLPawnExtensionComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	if (GetPawn<APawn>() == nullptr)
	{
		return;
	}

	RegisterInitStateFeature();

	TObjectPtr<UGameFrameworkComponentManager> manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}
