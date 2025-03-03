#include "CLAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CLAssetManager)

UCLAssetManager::UCLAssetManager() : Super()
{
	
}

bool UCLAssetManager::ShouldLogAssetLoads()
{
	/*
		VITCHAN, FCommandLine::Get() ���� ���� �����ų�� CommandLine�� ������ �� �ִ�.
	*/
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}
