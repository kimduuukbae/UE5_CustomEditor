// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlobalShaderTest.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FGlobalShaderTestModule"

void FGlobalShaderTestModule::StartupModule()
{
	FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GlobalShaderTest"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/GlobalShaderTest"), ShaderDir);
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGlobalShaderTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGlobalShaderTestModule, GlobalShaderTest)