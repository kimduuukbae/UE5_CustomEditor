#include "CLHeroComponent.h"
#include "CLPawnExtensionComponent.h"
#include "CustomLyra/CLGameplayTags.h"
#include "CustomLyra/Player/CLPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UCLHeroComponent::NAME_HeroFeatureName("Hero");

UCLHeroComponent::UCLHeroComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		return;
	}

	RegisterInitStateFeature();
}

void UCLHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UCLPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	TryToChangeInitState(FCLGameplayTags::Get().InitState_Spawned);

	CheckDefaultInitialization();
}

void UCLHeroComponent::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(InEndPlayReason);
}

void UCLHeroComponent::CheckDefaultInitialization()
{
	const FCLGameplayTags& initTags = FCLGameplayTags::Get();
	static const TArray<FGameplayTag> stateChain =
	{
		initTags.InitState_Spawned,
		initTags.InitState_DataAvailable,
		initTags.InitState_DataInitialized,
		initTags.InitState_GameplayReady
	};

	ContinueInitStateChain(stateChain);
}

void UCLHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& InParam)
{
	if (InParam.FeatureName != UCLPawnExtensionComponent::NAME_ActorFeatureName)
	{
		const FCLGameplayTags& initTags = FCLGameplayTags::Get();
		if (InParam.FeatureState == initTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UCLHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	const FCLGameplayTags& initTags = FCLGameplayTags::Get();
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	TObjectPtr<ACLPlayerState> playerState = GetPlayerState<ACLPlayerState>();

	if (CurrentState.IsValid() == false && DesiredState == initTags.InitState_Spawned)
	{
		if (IsValid(pawn) == true)
		{
			return true;
		}
	}

	if (CurrentState == initTags.InitState_Spawned && DesiredState == initTags.InitState_DataAvailable)
	{
		return IsValid(playerState);
	}

	if (CurrentState == initTags.InitState_DataAvailable && DesiredState == initTags.InitState_DataInitialized)
	{
		return IsValid(playerState) == true &&
			Manager->HasFeatureReachedInitState(pawn, UCLPawnExtensionComponent::NAME_ActorFeatureName, initTags.InitState_DataAvailable);
	}

	if (CurrentState == initTags.InitState_DataInitialized && DesiredState == initTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UCLHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FCLGameplayTags& initTags = FCLGameplayTags::Get();
	
	if (CurrentState == initTags.InitState_DataAvailable && DesiredState == initTags.InitState_DataInitialized)
	{
		TObjectPtr<APawn> pawn = GetPawn<APawn>();
		ACLPlayerState* playerState = GetPlayerState<ACLPlayerState>();
		if (IsValid(pawn) == false || IsValid(playerState) == false)
		{
			return;
		}

		/*const bool bIsLocallyControlled = pawn->IsLocallyControlled();
		TObjectPtr<const UCLPawnData> pawnData = nullptr;
		if (TObjectPtr<UCLPawnExtensionComponent> pawnExtensionComponent = UCLPawnExtensionComponent::FindPawnExtensionComponent(pawn))
		{
			pawnData = pawnExtensionComponent->GetPawnData<UCLPawnData>();
		}

		if (bIsLocallyControlled == true && IsValid(pawnData) == true)
		{
			if (TObjectPtr<UCLCameraComponent> cameraComponent = UCLCameraComponent::FindCameraComponent(pawn))
			{
				cameraComponent->DetermineCameraModeDelegate.BindUObject(this, &UCLHeroComponent::DetermineCameraMode);
			}
		}*/
	}
}
