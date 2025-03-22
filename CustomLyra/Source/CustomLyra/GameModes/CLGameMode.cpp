// Fill out your copyright notice in the Description page of Project Settings.

#include "CLGameMode.h"
#include "CLGameState.h"
#include "CLExperienceManagerComponent.h"
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
	// 우리는 StartPlay 에서 초기화 해서 Actor들이 BeginPlay가 불리기 전에 세팅하자

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

void ACLGameMode::OnExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> InDefinition)
{

}
