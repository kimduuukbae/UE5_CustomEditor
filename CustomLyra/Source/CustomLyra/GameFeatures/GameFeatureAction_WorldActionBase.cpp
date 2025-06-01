#include "GameFeatureAction_WorldActionBase.h"

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& InContext)
{
	for (const FWorldContext& worldContext : GEngine->GetWorldContexts())
	{
		if (InContext.ShouldApplyToWorldContext(worldContext) == true)
		{
			AddToWorld(worldContext, InContext);
		}
	}
}
