#pragma once

#include "Styling/SlateStyle.h"

class FCustomSlateStyleManager
{
public:
	static void Initialize();
	static void Shutdown();

private:
	static FName StyleSetName;
	static TSharedPtr<FSlateStyleSet> CreatedSlateStyleSet;

	static TSharedRef<FSlateStyleSet> CreateSlateStyleSet();

public:
	static FName GetStlyeSetName() { return StyleSetName; }

	static TSharedRef<FSlateStyleSet> GetCreatedSlateStyleSet() { return CreatedSlateStyleSet.ToSharedRef(); }
};