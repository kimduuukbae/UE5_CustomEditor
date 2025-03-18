// Fill out your copyright notice in the Description page of Project Settings.

#include "CLGameMode.h"

ACLGameMode::ACLGameMode(const FObjectInitializer& InObjectInitializer) : Super{InObjectInitializer}
{
}

void ACLGameMode::StartPlay()
{
	Super::StartPlay();
	// Lyra에서는 InitGame에서 해주고 있지만, 그건 Actor들의 PreInitializeComponents 가 불리기 전이라 그렇다.
	// 우리는 StartPlay 에서 초기화 해서 Actor들이 BeginPlay가 불리기 전에 세팅하자

}
