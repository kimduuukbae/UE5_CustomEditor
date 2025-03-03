// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomLyra.h"
#include "Modules/ModuleManager.h"

class FCLGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{

	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FCLGameModule, CustomLyra, "CustomLyra");
