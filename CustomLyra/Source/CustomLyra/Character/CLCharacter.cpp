#include "CLCharacter.h"
#include "CLPawnExtensionComponent.h"
#include "CustomLyra/Camera/CLCameraComponent.h"

ACLCharacter::ACLCharacter(const FObjectInitializer& InObjectInitilaizer) : Super{InObjectInitilaizer}
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtensionComponent = CreateDefaultSubobject<UCLPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	CameraComponent = CreateDefaultSubobject<UCLCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

void ACLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtensionComponent->SetupPlayerInputComponent();
}
