#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Delegates/Delegate.h"
#include "AsyncAction_ExperienceReady.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class UCLExperienceDefinition;

UCLASS()
class UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	virtual void Activate() override;

	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(TObjectPtr<const UCLExperienceDefinition> CurrentExperience);
	void Step4_BroadcastReady();

	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	TWeakObjectPtr<UWorld> WorldWeakPtr;
};
