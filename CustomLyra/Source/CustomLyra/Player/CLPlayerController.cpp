#include "CLPlayerController.h"
#include "CLPlayerState.h"
#include "CustomLyra/Camera/CLPlayerCameraManager.h"
#include "CustomLyra/AbilitySystem/CLAbilitySystemComponent.h"

ACLPlayerController::ACLPlayerController(const FObjectInitializer& InInitializer) : Super{InInitializer}
{
	PlayerCameraManagerClass = ACLPlayerCameraManager::StaticClass();
}

void ACLPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UCLAbilitySystemComponent* ASC = GetCLAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ACLPlayerState* ACLPlayerController::GetCLPlayerState() const
{
	return Cast<ACLPlayerState>(PlayerState);
}

UCLAbilitySystemComponent* ACLPlayerController::GetCLAbilitySystemComponent() const
{
	const ACLPlayerState* playerState = GetCLPlayerState();
	return IsValid(playerState) == true ? playerState->GetCLAbilitySystemComponent() : nullptr;
}
