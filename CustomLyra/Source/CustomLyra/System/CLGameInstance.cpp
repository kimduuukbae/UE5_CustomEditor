#include "CLGameInstance.h"
#include "CustomLyra/CLGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

UCLGameInstance::UCLGameInstance(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

void UCLGameInstance::Init()
{
	Super::Init();

	TObjectPtr<UGameFrameworkComponentManager> componentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (IsValid(componentManager) == true)
	{
		const FCLGameplayTags tags = FCLGameplayTags::Get();

		componentManager->RegisterInitState(tags.InitState_Spawned, false, FGameplayTag());
		componentManager->RegisterInitState(tags.InitState_DataAvailable, false, tags.InitState_Spawned);
		componentManager->RegisterInitState(tags.InitState_DataInitialized, false, tags.InitState_DataAvailable);
		componentManager->RegisterInitState(tags.InitState_GameplayReady, false, tags.InitState_DataInitialized);
	}
}

void UCLGameInstance::Shutdown()
{
	Super::Shutdown();
}
