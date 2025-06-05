#include "CLControllerComponent_CharacterParts.h"
#include "CLPawnComponent_CharacterParts.h"

UCLControllerComponent_CharacterParts::UCLControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
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
