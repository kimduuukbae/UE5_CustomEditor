#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "CLHeroComponent.generated.h"

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UCLHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UCLHeroComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
};