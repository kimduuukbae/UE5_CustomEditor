#include "CLPlayerState.h"
#include "CustomLyra/GameModes/CLExperienceManagerComponent.h"
#include "CustomLyra/GameModes/CLGameMode.h"
#include "CustomLyra/AbilitySystem/CLAbilitySystemComponent.h"
#include "CustomLyra/AbilitySystem/CLAbilitySet.h"

ACLPlayerState::ACLPlayerState(const FObjectInitializer& InObjectInitializer) : Super{InObjectInitializer}
{
	AbilitySystemComponent = InObjectInitializer.CreateDefaultSubobject<UCLAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void ACLPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// PlayerController가 만들어지고 나서, AbilitySystemComponent가 CreateDefaultSubobject로 인해 붙었으므로
	// APlayerState가 InitializeComponent 할 때 AbilitySystemComponent의 InitializeComponent가 불리게 되면서
	// 그 해당 내부에서 InitAbilityActorInfo를 세팅해준다. 그것을 일단 다시 nullptr로 바꾼다.
	FGameplayAbilityActorInfo* actorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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
	if (TObjectPtr<ACLGameMode> gameMode = GetWorld()->GetAuthGameMode<ACLGameMode>())
	{
		const UCLPawnData* newPawnData = gameMode->GetPawnDataForController(GetOwningController());

		SetPawnData(newPawnData);
	}
}

void ACLPlayerState::SetPawnData(TObjectPtr<const UCLPawnData> InPawnData)
{
	PawnData = InPawnData;

	for (UCLAbilitySet* abilitySet : PawnData->AbilitySet)
	{
		if (IsValid(abilitySet) == true)
		{
			abilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
