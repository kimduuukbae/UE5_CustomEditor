#include "GameFeatureAction_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "Components/GAmeFrameworkComponentManager.h"
#include "CustomLyra/Character/CLHeroComponent.h"

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& InContext)
{
	FPerContextData& activeData = ContextData.FindOrAdd(InContext);
	if (activeData.ExtensionRequestHandles.IsEmpty() == false ||
		activeData.PawnsAddedTo.IsEmpty() == false)
	{
		Reset(activeData);
	}

	Super::OnGameFeatureActivating(InContext);
}

void UGameFeatureAction_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& InContext)
{
	Super::OnGameFeatureDeactivating(InContext);

	FPerContextData* activeData = ContextData.Find(InContext);
	if (activeData != nullptr)
	{
		Reset(*activeData);
	}
}

void UGameFeatureAction_AddInputConfig::AddToWorld(const FWorldContext& InWorldContext, const FGameFeatureStateChangeContext& InChangeContext)
{
	UWorld* world = InWorldContext.World();
	UGameInstance* gameInstance = InWorldContext.OwningGameInstance;
	FPerContextData& activeData = ContextData.FindOrAdd(InChangeContext);
	if (IsValid(gameInstance) == true && IsValid(world) == true && world->IsGameWorld() == true)
	{
		if (UGameFrameworkComponentManager* componentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			// InChangeContext, 즉 해당 World가 변화가 있을때마다 delegate가 호출될 것이다.
			// -> 우리의 GameFeatureState가 변경될 때 콜백됨
			UGameFrameworkComponentManager::FExtensionHandlerDelegate delegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &UGameFeatureAction_AddInputConfig::HandlePawnExtension, InChangeContext);

			TSharedPtr<FComponentRequestHandle> handle = componentManager->AddExtensionHandler(APawn::StaticClass(), delegate);
			activeData.ExtensionRequestHandles.Add(handle);
		}
	}
}

void UGameFeatureAction_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	APawn* pawn = Cast<APawn>(Actor);
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);
	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UCLHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(pawn, activeData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UCLHeroComponent::NAME_BindInputsNow)
	{
		RemoveInputConfig(pawn, activeData);
	}
}

void UGameFeatureAction_AddInputConfig::AddInputConfig(APawn* InPawn, FPerContextData& ActiveData)
{
	APlayerController* controller = Cast<APlayerController>(InPawn->GetController());
	if (ULocalPlayer* localPlayer = IsValid(controller) == true ? controller->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions options{};
			options.bIgnoreAllPressedKeysUntilRelease = false;

			for (const FCLMappableConfigPair& pair : InputConfigs)
			{
				subSystem->AddPlayerMappableConfig(pair.Config.LoadSynchronous(), options);
			}

			ActiveData.PawnsAddedTo.AddUnique(InPawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::RemoveInputConfig(APawn* InPawn, FPerContextData& ActiveData)
{
	APlayerController* controller = Cast<APlayerController>(InPawn->GetController());
	if (ULocalPlayer* localPlayer = IsValid(controller) == true ? controller->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (const FCLMappableConfigPair& pair : InputConfigs)
			{
				subSystem->RemovePlayerMappableConfig(pair.Config.LoadSynchronous());
			}

			ActiveData.PawnsAddedTo.Remove(InPawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while (ActiveData.PawnsAddedTo.IsEmpty() == false)
	{
		TWeakObjectPtr<APawn> pawnWeakPtr = ActiveData.PawnsAddedTo.Top();
		if (pawnWeakPtr.IsValid() == true)
		{
			RemoveInputConfig(pawnWeakPtr.Get(), ActiveData);
		}
		else
		{
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}
