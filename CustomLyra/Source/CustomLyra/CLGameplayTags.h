#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FCLGameplayTags
{
	static const FCLGameplayTags& Get() { return GameplayTags; }
	static void InitailizeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;

private:
	static FCLGameplayTags GameplayTags;
};