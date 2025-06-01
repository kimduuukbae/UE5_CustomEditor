#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "CustomLyra/Input/CLMappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;

UCLASS()
class UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& InContext) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& InContext) override;

	virtual void AddToWorld(const FWorldContext& InWorldContext, const FGameFeatureStateChangeContext& InChangeContext) override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	void AddInputConfig(APawn* InPawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* InPawn, FPerContextData& ActiveData);

	void Reset(FPerContextData& ActiveData);

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	UPROPERTY(EditAnywhere)
	TArray<FCLMappableConfigPair> InputConfigs;
};