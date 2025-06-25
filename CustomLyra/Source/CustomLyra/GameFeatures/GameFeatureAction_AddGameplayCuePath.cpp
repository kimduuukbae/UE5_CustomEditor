#include "GameFeatureAction_AddGameplayCuePath.h"

UGameFeatureAction_AddGameplayCuePath::UGameFeatureAction_AddGameplayCuePath() : Super{}
{
	DirectoryPathsToAdd.Add(FDirectoryPath{ TEXT("/GameplayCues") });
}
