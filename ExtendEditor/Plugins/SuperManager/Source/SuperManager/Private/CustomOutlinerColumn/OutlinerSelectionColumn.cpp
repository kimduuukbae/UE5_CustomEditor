#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/CustomSlateStyle.h"

SHeaderRow::FColumn::FArguments FOutlinerSelectionLockColumn::ConstructHeaderRowColumn()
{
	SHeaderRow::FColumn::FArguments HeaderRow = SHeaderRow::Column(GetColumnID())
	.FixedWidth(24.0f)
	.HAlignHeader(HAlign_Center)
	.VAlignHeader(VAlign_Center)
	.HAlignCell(HAlign_Center)
	.VAlignCell(VAlign_Center)
	.DefaultTooltip(FText::FromString(TEXT("Actor Selection Lock - Press icon to lock actor")))
	[
		SNew(SImage)
		.ColorAndOpacity(FSlateColor::UseForeground())
		.Image(FCustomSlateStyleManager::GetCreatedSlateStyleSet()->GetBrush(FName("Editor.LockIcon")))
	];
	
	return HeaderRow;
}

const TSharedRef<SWidget> FOutlinerSelectionLockColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row)
{
	return SNullWidget::NullWidget;
}
