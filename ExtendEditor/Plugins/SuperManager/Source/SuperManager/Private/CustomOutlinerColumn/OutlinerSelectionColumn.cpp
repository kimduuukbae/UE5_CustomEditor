#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/CustomSlateStyle.h"
#include "ActorTreeItem.h"
#include "SuperManager.h"

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
	FActorTreeItem* ActorTreeItem = TreeItem->CastTo<FActorTreeItem>();

	if (ActorTreeItem == nullptr || ActorTreeItem->IsValid() == false)
	{
		return SNullWidget::NullWidget;
	}

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");
	bool bIsActorLocked = SuperManagerModule.CheckIsActorLocked(ActorTreeItem->Actor.Get());

	TSharedRef<SCheckBox> RowWidget = SNew(SCheckBox)
	.Visibility(EVisibility::Visible)
	.HAlign(HAlign_Center)
	.IsChecked(bIsActorLocked == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
	.OnCheckStateChanged(this, &FOutlinerSelectionLockColumn::OnCheckBoxStateChanged, ActorTreeItem->Actor);

	return RowWidget;
}

void FOutlinerSelectionLockColumn::OnCheckBoxStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> Target)
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");

	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		SuperManagerModule.ProcessLockingForOutliner(Target.Get(), false);
		break;
	case ECheckBoxState::Checked:
		SuperManagerModule.ProcessLockingForOutliner(Target.Get(), true);
		break;
	default:
		break;
	}
}
