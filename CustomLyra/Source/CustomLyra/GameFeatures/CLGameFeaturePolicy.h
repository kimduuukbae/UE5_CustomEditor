#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "CLGameFeaturePolicy.generated.h"


// GameFeature Plugin이 Memory에 로딩되거나 Active를 관리하는 StateMachine을 모니터링 할수있다.
UCLASS()
class UCLGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
public:
	UCLGameFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

/*
	GameFeature Plugin이 Register / UnRegister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 GameplayCue 에셋을 스캔할 수 있도록 해야함
	이를 위해 GameFeatureStateChangeObserver를 활용하여 가능하게 한다.
*/

UCLASS()
class UCLGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};