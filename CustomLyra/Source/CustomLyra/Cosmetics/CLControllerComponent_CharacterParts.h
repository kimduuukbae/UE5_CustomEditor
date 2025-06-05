#pragma once

#include "Components/ControllerComponent.h"
#include "CLCharacterPartTypes.h"
#include "CLControllerComponent_CharacterParts.generated.h"

class UCLPawnComponent_CharacterParts;

USTRUCT()
struct FCLControllerCharacterPartEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FCLCharacterPart Part;

	FCLCharacterPartHandle Handle;
};

// Controller가 pawn 을 possess 했을 때, pawn 에 어떤 costmetic actor를 생성할지 결정하는 controller component
UCLASS(meta = (BlueprintSpawnableComponent))
class UCLControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UCLControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FCLCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	void AddCharacterPartInternal(const FCLCharacterPart& NewPart);
	UCLPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FCLControllerCharacterPartEntry> CharacterParts;
};