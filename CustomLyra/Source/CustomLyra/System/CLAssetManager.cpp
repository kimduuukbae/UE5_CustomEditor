#include "CLAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CLAssetManager)

UCLAssetManager::UCLAssetManager() : Super()
{
	
}

bool UCLAssetManager::ShouldLogAssetLoads()
{
	/*
		VITCHAN, FCommandLine::Get() 으로 현재 실행시킬때 CommandLine을 가져올 수 있다.
	*/
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}
