#include "CustomStyle/CustomSlateStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

FName FCustomSlateStyleManager::StyleSetName = FName("CustomSlateStyle");
TSharedPtr<FSlateStyleSet> FCustomSlateStyleManager::CreatedSlateStyleSet = nullptr;

void FCustomSlateStyleManager::Initialize()
{
	if (CreatedSlateStyleSet.IsValid() == false)
	{
		CreatedSlateStyleSet = CreateSlateStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet);
	}
}

void FCustomSlateStyleManager::Shutdown()
{
}

TSharedRef<FSlateStyleSet> FCustomSlateStyleManager::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	const FString IconDir = IPluginManager::Get().FindPlugin(TEXT("SuperManager"))->GetBaseDir() / "Resources";

	CustomStyleSet->SetContentRoot(IconDir);

	const FVector2D IconSize(16.0f, 16.0f);

	CustomStyleSet->Set("Editor.LockIcon", new FSlateImageBrush(IconDir / "EditorLock.png", IconSize));

	return CustomStyleSet;
}
