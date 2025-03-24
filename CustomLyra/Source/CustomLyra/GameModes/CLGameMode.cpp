// Fill out your copyright notice in the Description page of Project Settings.

#include "CLGameMode.h"
#include "CLGameState.h"
#include "CLExperienceManagerComponent.h"
#include "CLExperienceDefinition.h"
#include "CustomLyra/Player/CLPlayerController.h"
#include "CustomLyra/Player/CLPlayerState.h"
#include "CustomLyra/Character/CLCharacter.h" 

ACLGameMode::ACLGameMode(const FObjectInitializer& InObjectInitializer) : Super{InObjectInitializer}
{
	GameStateClass = ACLGameState::StaticClass();
	PlayerControllerClass = ACLPlayerController::StaticClass();
	DefaultPawnClass = ACLCharacter::StaticClass();
	PlayerStateClass = ACLPlayerState::StaticClass();
}

void ACLGameMode::InitGameState()
{
	Super::InitGameState();

	TObjectPtr<UCLExperienceManagerComponent> managerComponent = GameState->FindComponentByClass<UCLExperienceManagerComponent>();
	managerComponent->CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate::CreateUObject(this, &ACLGameMode::OnExperienceLoaded));
}

void ACLGameMode::StartPlay()
{
	Super::StartPlay();
	// Lyra에서는 InitGame에서 해주고 있지만, 그건 Actor들의 PreInitializeComponents 가 불리기 전이라 그렇다.
	// 즉, GameState, PlayerController, PlayerState도 만들어져야 하니까..
	// 우리는 StartPlay 에서 초기화 해서 Actor들이 BeginPlay가 불리기 전에 세팅하자

	FPrimaryAssetId experienceId;
	TObjectPtr<UWorld> world = GetWorld();
	if (IsValid(world) == false)
	{
		return;
	}
	
	if (experienceId.IsValid() == false)
	{
		experienceId = FPrimaryAssetId(FPrimaryAssetType("CLExperienceDefinition"), FName(TEXT("B_CLDefaultExperience")));
	}

	OnMatchAssignmentGiven(experienceId);
}

APawn* ACLGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ACLGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded() == true)
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

UClass* ACLGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UCLPawnData* pawnData = GetPawnDataForController(InController))
	{
		if (pawnData->PawnClass)
		{
			return pawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACLGameMode::OnMatchAssignmentGiven(const FPrimaryAssetId& InExperienceId)
{
	if (InExperienceId.IsValid() == false)
	{
		return;
	}

	TObjectPtr<UCLExperienceManagerComponent> experienceManagerComponent = GameState->FindComponentByClass<UCLExperienceManagerComponent>();
	if (IsValid(experienceManagerComponent) == false)
	{
		return;
	}

	experienceManagerComponent->ServerSetCurrentExperience(InExperienceId);
}

bool ACLGameMode::IsExperienceLoaded() const
{
	if (IsValid(GameState) == false)
	{
		return false;
	}

	TObjectPtr<UCLExperienceManagerComponent> managerComponent = GameState->FindComponentByClass<UCLExperienceManagerComponent>();
	if (IsValid(managerComponent) == false)
	{
		return false;
	}

	return managerComponent->IsExperienceLoaded();
}

TObjectPtr<const UCLPawnData> ACLGameMode::GetPawnDataForController(TObjectPtr<const AController> InController)
{
	if (IsValid(InController) == true)
	{
		if (TObjectPtr<const ACLPlayerState> playerState = InController->GetPlayerState<ACLPlayerState>())
		{
			if (TObjectPtr<const UCLPawnData> pawnData = playerState->GetPawnData<UCLPawnData>())
			{
				return pawnData;
			}
		}
	}

	TObjectPtr<UCLExperienceManagerComponent> managerComponent = GameState->FindComponentByClass<UCLExperienceManagerComponent>();
	if (managerComponent->IsExperienceLoaded() == true)
	{
		TObjectPtr<const UCLExperienceDefinition> experience = managerComponent->GetCurrentExperienceChecked();
		if (experience->DefaultPawnData)
		{
			return experience->DefaultPawnData;
		}
	}

	return nullptr;
}

void ACLGameMode::OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition)
{
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		TObjectPtr<APlayerController> controller = Cast<APlayerController>(*iter);

		if (IsValid(controller) == true && controller->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(controller) == true)
			{
				RestartPlayer(controller);
			}
		}
	}
}
