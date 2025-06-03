#include "ModularPlayerController.h"
#include "Components/GameFrameworkComponentManager.h"

void AModularPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerController::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EEndPlayReason);
}
