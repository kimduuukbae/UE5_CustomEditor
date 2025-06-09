#include "AsyncAction_ExperienceReady.h"
#include "CLExperienceManagerComponent.h"
#include "CLExperienceDefinition.h"

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* action = nullptr;
	if (UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		action = NewObject<UAsyncAction_ExperienceReady>();
		action->WorldWeakPtr = world;
		action->RegisterWithGameInstance(world);
	}

	return action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* world = WorldWeakPtr.Get())
	{
		if (AGameStateBase* gameState = world->GetGameState())
		{
			Step2_ListenToExperienceLoading(gameState);
		}
		else
		{
			world->GameStateSetEvent.AddUObject(this, &UAsyncAction_ExperienceReady::Step1_HandleGameStateSet);
		}
	}
	else
	{
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* world = WorldWeakPtr.Get())
	{
		world->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	UCLExperienceManagerComponent* experienceManagerComponent = GameState->FindComponentByClass<UCLExperienceManagerComponent>();

	if (experienceManagerComponent->IsExperienceLoaded() == true)
	{
		if (UWorld* world = GameState->GetWorld())
		{
			world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAsyncAction_ExperienceReady::Step4_BroadcastReady));
		}
	}
	else
	{
		experienceManagerComponent->CallOrRegister_OnExperienceLoaded(FCLOnExperienceLoaded::FDelegate::CreateUObject(this, &UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> CurrentExperience)
{
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	OnReady.Broadcast();
	SetReadyToDestroy();
}
