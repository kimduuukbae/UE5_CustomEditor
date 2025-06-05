#pragma once

#include "GameFramework/Actor.h"
#include "CLTaggedActor.generated.h"

UCLASS()
class ACLTaggedActor : public AActor
{
	GENERATED_BODY()
public:
	ACLTaggedActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};