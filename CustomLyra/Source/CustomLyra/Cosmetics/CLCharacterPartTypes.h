#pragma once

#include "CoreMinimal.h"
#include "CLCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FCLCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const 
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FCLCharacterPart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};
