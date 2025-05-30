// Fill out your copyright notice in the Description page of Project Settings.

#include "CLUserFacingExperienceDefinition.h"
#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* UCLUserFacingExperienceDefinition::CreateHostingRequest() const
{
    const FString experienceName = ExperienceId.PrimaryAssetName.ToString();

    UCommonSession_HostSessionRequest* result = NewObject<UCommonSession_HostSessionRequest>();
    result->MapId = MapId;
    result->ExtraArgs.Add(TEXT("Experience"), experienceName);
    
    return result;
}
