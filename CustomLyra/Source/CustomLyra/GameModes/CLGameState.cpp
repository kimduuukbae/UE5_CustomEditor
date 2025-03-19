#include "CLGameState.h"
#include "CLExperienceManagerComponent.h"

ACLGameState::ACLGameState(const FObjectInitializer& InObjectInitializer) : Super{InObjectInitializer}
{
	ExperienceManagerComponent = CreateDefaultSubobject<UCLExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
