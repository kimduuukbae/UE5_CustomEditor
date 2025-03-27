#include "CLPawnExtensionComponent.h"
#include "CustomLyra/CLGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UCLPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

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

void UCLPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	TryToChangeInitState(FCLGameplayTags::Get().InitState_Spawned);

	CheckDefaultInitialization();
}

void UCLPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UCLPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
}

bool UCLPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return false;
}

void UCLPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();

	const FCLGameplayTags& initTags = FCLGameplayTags::Get();

	static const TArray<FGameplayTag> stateChain =
	{
		initTags.InitState_Spawned,
		initTags.InitState_DataAvailable,
		initTags.InitState_DataInitialized,
		initTags.InitState_Spawned
	};

	ContinueInitStateChain(stateChain);
}
