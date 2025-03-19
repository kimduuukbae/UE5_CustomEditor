#include "CLExperienceManagerComponent.h"
#include "CLExperienceDefinition.h"

UCLExperienceManagerComponent::UCLExperienceManagerComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

void UCLExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate&& InDelegate)
{
	if (IsExperienceLoaded())
	{
		InDelegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(InDelegate));
	}
}
