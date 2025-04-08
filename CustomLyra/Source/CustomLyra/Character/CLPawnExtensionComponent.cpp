#include "CLPawnExtensionComponent.h"
#include "CustomLyra/CLGameplayTags.h"
#include "CLPawnData.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UCLPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UCLPawnExtensionComponent::UCLPawnExtensionComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLPawnExtensionComponent::SetPawnData(TObjectPtr<const UCLPawnData> InPawnData)
{
	TObjectPtr<APawn> pawn = GetPawnChecked<APawn>();
	if (pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (IsValid(PawnData) == true)
	{
		return;
	}

	PawnData = InPawnData;

	// CheckDefaultInitialization();
	// Lyra는 여기에 해당 함수를 넣어주고 있는데, 사실 불릴 일이 없다.
	// 왜냐면 SetPawnData가 호출되는 시점은 GameMode::SpawnDefaultPawnAtTransform_Impl에 의해서 Actor가 생성되는데
	// 이땐 Possess 되어있지 않다. 그래서 bIsLocallyControlled 는 항상 false다.
	// Possess는 RestartPlayer->FinishRestartPlayer 후에 NewPlayer->Possess로 인해 일어난다.

	/*
		Flow : GameMode::RestartPlayer -> RestartPlayerAtPlayerStart
		RestartPlayerAtPlayerState -> SpawnDefaultPawnAtTransform 에 의해 CLGameMode Impl 에서 SetPawnData (어차피 FinishSpawning 후에 BeginPlay가 불린다는 것을 명심하자)
		그 후 RestartPlayerAtPlayerState -> FinishRestartPlayer 
		이 때 FinishRestartPlayer 함수에서 Possess가 일어난다
	*/
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
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FCLGameplayTags& initTags = FCLGameplayTags::Get();
		if (Params.FeatureState == initTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UCLPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	const FCLGameplayTags& initTags = FCLGameplayTags::Get();

	// InitState_Spawned
	if (CurrentState.IsValid() == false && DesiredState == initTags.InitState_Spawned)
	{
		if (IsValid(pawn) == true)
		{
			return true;
		}
	}

	// InitState_Spawned -> InitState_DataAvailable
	if (CurrentState == initTags.InitState_Spawned && DesiredState == initTags.InitState_DataAvailable)
	{
		if (PawnData == nullptr)
		{
			return false;
		}

		// 싱글에서 Authority 있는 것은 내가 소유하고 있는 MainController가 있느냐를 체크
		// 즉 내가 소유하고 있지만, 조종하고 있지 않는 애들은 Authority가 false로 나옴
		const bool bHasAuthority = pawn->HasAuthority();
		const bool bIsLocallyControlled = pawn->IsLocallyControlled();
		if (bHasAuthority == true || bIsLocallyControlled == true)
		{
			// 해당 Component를 가지고 있는 pawn은 아직 누군가에게 possess가 안됐다는 것
			if (GetController<AController>() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	if (CurrentState == initTags.InitState_DataAvailable && DesiredState == initTags.InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(pawn, initTags.InitState_DataAvailable);
	}

	if (CurrentState == initTags.InitState_DataInitialized && DesiredState == initTags.InitState_GameplayReady)
	{
		return true;
	}

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
