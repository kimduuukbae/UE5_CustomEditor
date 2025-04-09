#include "CLCharacter.h"
#include "CLPawnExtensionComponent.h"

ACLCharacter::ACLCharacter(const FObjectInitializer& InObjectInitilaizer) : Super{InObjectInitilaizer}
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtensionComponent = CreateDefaultSubobject<UCLPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void ACLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtensionComponent->SetupPlayerInputComponent();
}
