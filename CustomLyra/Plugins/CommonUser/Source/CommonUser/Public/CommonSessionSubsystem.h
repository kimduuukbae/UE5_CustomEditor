#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

class APlayerController;

UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:

	FString GetMapName() const;
	FString ConstructTravelURL() const;

	UPROPERTY(BlueprintReadWrite, Category = Session)
	FPrimaryAssetId MapId;

	UPROPERTY(BlueprintReadWrite, Category = Session)
	TMap<FString, FString> ExtraArgs;
};

// 다른 모듈에서 해당 Plugin을 쓰게 하고싶다면 COMMONUSER_API 를 해서 declspec(export) 하게 해야함
UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonSessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category = Session)
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

	FString PendingTravelURL = TEXT("");
};