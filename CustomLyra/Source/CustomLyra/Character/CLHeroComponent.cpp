#include "CLHeroComponent.h"
#include "CLPawnExtensionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CustomLyra/CLGameplayTags.h"
#include "CustomLyra/Player/CLPlayerState.h"
#include "CustomLyra/Player/CLPlayerController.h"
#include "CustomLyra/Character/CLPawnData.h"
#include "CustomLyra/Camera/CLCameraComponent.h"
#include "CustomLyra/Input/CLMappableConfigPair.h"
#include "CustomLyra/Input/CLInputComponent.h"
#include "CustomLyra/AbilitySystem/CLAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UCLHeroComponent::NAME_HeroFeatureName("Hero");
const FName UCLHeroComponent::NAME_BindInputsNow("BindInputsNow");

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

		const bool bIsLocallyControlled = pawn->IsLocallyControlled();
		TObjectPtr<const UCLPawnData> pawnData = nullptr;
		if (TObjectPtr<UCLPawnExtensionComponent> pawnExtensionComponent = UCLPawnExtensionComponent::FindPawnExtensionComponent(pawn))
		{
			pawnData = pawnExtensionComponent->GetPawnData<UCLPawnData>();

			pawnExtensionComponent->InitializeAbilitySystem(playerState->GetCLAbilitySystemComponent(), playerState);
		}

		if (bIsLocallyControlled == true && IsValid(pawnData) == true)
		{
			if (TObjectPtr<UCLCameraComponent> cameraComponent = UCLCameraComponent::FindCameraComponent(pawn))
			{
				cameraComponent->DetermineCameraModeDelegate.BindUObject(this, &UCLHeroComponent::DetermineCameraMode);
			}
		}


		if (TObjectPtr<ACLPlayerController> CLPlayerController = GetController<ACLPlayerController>())
		{
			if (IsValid(pawn->InputComponent) == true)
			{
				InitializePlayerInput(pawn->InputComponent);
			}
		}
	}
}

TSubclassOf<UCLCameraMode> UCLHeroComponent::DetermineCameraMode() const
{
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	if (IsValid(pawn) == false)
	{
		return nullptr;
	}

	if (TObjectPtr<UCLPawnExtensionComponent> pawnExtensionComponent = UCLPawnExtensionComponent::FindPawnExtensionComponent(pawn))
	{
		if (TObjectPtr<const UCLPawnData> pawnData = pawnExtensionComponent->GetPawnData<UCLPawnData>())
		{
			return pawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UCLHeroComponent::InitializePlayerInput(TObjectPtr<UInputComponent> InInputComponent)
{
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	if (IsValid(pawn) == false)
	{
		return;
	}

	TObjectPtr<const APlayerController> controller = GetController<APlayerController>();
	if (IsValid(controller) == false)
	{
		return;
	}

	TObjectPtr<const ULocalPlayer> localPlayer = controller->GetLocalPlayer();
	if (IsValid(localPlayer) == false)
	{
		return;
	}

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> subSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(subSystem) == false)
	{
		return;
	}

	subSystem->ClearAllMappings();

	// ConfigData를 PawnData에 있었고, PawnData는 PawnExtensionComponent의 Experience에 지정되어 있으니
	if (TObjectPtr<const UCLPawnExtensionComponent> pawnExtensionComponent = UCLPawnExtensionComponent::FindPawnExtensionComponent(pawn))
	{
		if (TObjectPtr<const UCLPawnData> pawnData = pawnExtensionComponent->GetPawnData<UCLPawnData>())
		{
			if (TObjectPtr<const UCLInputConfig> inputConfig = pawnData->InputConfig)
			{
				const FCLGameplayTags& gameplayTags = FCLGameplayTags::Get();

				for (const FCLMappableConfigPair& pair : DefaultInputConfigs)
				{
					if (pair.bShouldActivateAutomatically == true)
					{
						FModifyContextOptions options{};
						options.bIgnoreAllPressedKeysUntilRelease = false;

						subSystem->AddPlayerMappableConfig(pair.Config.LoadSynchronous(), options);
					}
				}

				TObjectPtr<UCLInputComponent> inputComponent = CastChecked<UCLInputComponent>(InInputComponent);
				if (IsValid(inputComponent) == true)
				{
					inputComponent->BindNativeAction(inputConfig, gameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &UCLHeroComponent::Input_Move);
					inputComponent->BindNativeAction(inputConfig, gameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &UCLHeroComponent::Input_LookMouse);
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(pawn, NAME_BindInputsNow);
}

void UCLHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	if (IsValid(pawn) == false)
	{
		return;
	}

	TObjectPtr<AController> controller = pawn->GetController();
	if (IsValid(controller) == false)
	{
		return;
	}

	FVector2D value = InputActionValue.Get<FVector2D>();
	FRotator movementRotation(0.0f, controller->GetControlRotation().Yaw, 0.0f);

	if (FMath::IsNearlyZero(value.X) == false)
	{
		FVector movementDirection = movementRotation.RotateVector(FVector::RightVector);
		pawn->AddMovementInput(movementDirection, value.X);
	}
	
	if (FMath::IsNearlyZero(value.Y) == false)
	{
		FVector movementDirection = movementRotation.RotateVector(FVector::ForwardVector);
		pawn->AddMovementInput(movementDirection, value.Y);
	}
}

void UCLHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	TObjectPtr<APawn> pawn = GetPawn<APawn>();
	if (IsValid(pawn) == false)
	{
		return;
	}

	const FVector2D value = InputActionValue.Get<FVector2D>();
	if (FMath::IsNearlyZero(value.X) == false)
	{
		pawn->AddControllerYawInput(value.X);
	}

	if (FMath::IsNearlyZero(value.Y) == false)
	{
		pawn->AddControllerPitchInput(-value.Y);
	}
}
