#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHeader 
{
	static void Print(const FString& Message, const FColor& Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, Color, Message);
		}
	}

	static void PrintLog(const FString& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}

	static EAppReturnType::Type ShowMessageDialog(EAppMsgType::Type MessageType, const FString& Message, bool bShowMessageAsWarning = true)
	{
		if (bShowMessageAsWarning)
		{
			FText MsgTitle = FText::FromString(TEXT("Warning"));
			return FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message), &MsgTitle);
		}

		return FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
	}

	static void ShowNotifyInfo(const FString& Message)
	{
		FNotificationInfo NotifyInfo = FText::FromString(Message);
		NotifyInfo.bUseLargeFont = true;
		NotifyInfo.FadeOutDuration = 7.0f;

		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}

