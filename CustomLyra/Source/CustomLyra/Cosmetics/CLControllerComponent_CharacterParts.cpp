#include "CLControllerComponent_CharacterParts.h"
#include "CLPawnComponent_CharacterParts.h"

UCLControllerComponent_CharacterParts::UCLControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == true)
	{
		if (AController* controller = GetController<AController>())
		{
			controller->OnPossessedPawnChanged.AddDynamic(this, &UCLControllerComponent_CharacterParts::OnPossessedPawnChanged);
		}
	}
}

void UCLControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

void UCLControllerComponent_CharacterParts::AddCharacterPart(const FCLCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UCLControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UCLPawnComponent_CharacterParts* pawnCustomizer = GetPawnCustomizer())
	{
		for (FCLControllerCharacterPartEntry& entry : CharacterParts)
		{
			pawnCustomizer->RemoveCharacterPart(entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UCLControllerComponent_CharacterParts::AddCharacterPartInternal(const FCLCharacterPart& NewPart)
{
	FCLControllerCharacterPartEntry& newEntry = CharacterParts.AddDefaulted_GetRef();
	newEntry.Part = NewPart;

	if (UCLPawnComponent_CharacterParts* pawnCustomizer = GetPawnCustomizer())
	{
		newEntry.Handle = pawnCustomizer->AddCharacterPart(NewPart);
	}
}

UCLPawnComponent_CharacterParts* UCLControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* controlledPawn = GetPawn<APawn>())
	{
		return controlledPawn->FindComponentByClass<UCLPawnComponent_CharacterParts>();
	}

	return nullptr;
}

void UCLControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UCLPawnComponent_CharacterParts* oldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UCLPawnComponent_CharacterParts>() : nullptr)
	{
		for (FCLControllerCharacterPartEntry& entry : CharacterParts)
		{
			oldCustomizer->RemoveCharacterPart(entry.Handle);
			entry.Handle.Reset();
		}
	}

	if (UCLPawnComponent_CharacterParts* newCustomizer = NewPawn ? NewPawn->FindComponentByClass<UCLPawnComponent_CharacterParts>() : nullptr)
	{
		for (FCLControllerCharacterPartEntry& entry : CharacterParts)
		{
			entry.Handle = newCustomizer->AddCharacterPart(entry.Part);
		}
	}
}
