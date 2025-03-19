#include "CLPlayerState.h"
#include "CustomLyra/GameModes/CLExperienceManagerComponent.h"

ACLPlayerState::ACLPlayerState(const FObjectInitializer& InObjectInitializer) : Super{InObjectInitializer}
{
}

void ACLPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TObjectPtr<AGameStateBase> gameState = GetWorld()->GetGameState();
	if (gameState == nullptr)
	{
		return;
	}

	TObjectPtr<UCLExperienceManagerComponent> managerComponent = gameState->FindComponentByClass<UCLExperienceManagerComponent>();
	if (managerComponent == nullptr)
	{
		return;
	}

	// BP의 AsyncLoad로 인해서 BeginPlay 후에 ExperienceDefinition이 생성된다. 이를 위해서 얘도 Bind
	managerComponent->CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate::CreateUObject(this, &ACLPlayerState::OnExperienceLoaded));
}

void ACLPlayerState::OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition)
{

}
