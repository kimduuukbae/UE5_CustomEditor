#include "CommonSessionSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Engine/AssetManager.h"

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	FAssetData mapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapId, mapAssetData) == true)
	{
		return mapAssetData.PackageName.ToString();
	}

	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString combinedExtraArgs;

	for (const auto& [key, value] : ExtraArgs)
	{
		if (key.IsEmpty() == true)
		{
			continue;
		}

		if (value.IsEmpty() == true)
		{
			combinedExtraArgs += FString::Printf(TEXT("?%s"), *key);
		}
		else
		{
			combinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *key, *value);
		}
	}

	return FString::Printf(TEXT("%s%s"), *GetMapName(), *combinedExtraArgs);
}

void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	TObjectPtr<ULocalPlayer> localPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (IsValid(localPlayer) == false)
	{
		return;
	}

	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}