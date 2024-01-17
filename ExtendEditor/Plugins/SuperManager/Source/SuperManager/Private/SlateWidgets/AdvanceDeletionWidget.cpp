#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "SuperManager.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;

	StoredAssetsData = InArgs._AssetsDataToStore;

	ChildSlot
		[
			// Main vertical box
			SNew(SVerticalBox)

			//First vertical slot for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Advance Deletion")))
		.Font(TitleTextFont)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FColor::White)
		]

		// Second Slot
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]

		// Third slot
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)

			+ SScrollBox::Slot()
		[
			ConstructAssetListView()
		]
		]

		// Fourth Slot
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.Padding(5.0f)
			.FillWidth(10.0f)
			[
				// Delete All
				ConstructDeleteAllButton()
			]

			+ SHorizontalBox::Slot()
			.Padding(5.0f)
			.FillWidth(10.0f)
			[
				// Select All
				ConstructSelectAllButton()
			]

			+ SHorizontalBox::Slot()
			.Padding(5.0f)
			.FillWidth(10.0f)
			[
				// UnSelect All
				ConstructUnSelectAllButton()
			]
		]
	];
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetListView()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
	.ItemHeight(24.0f)
	.ListItemsSource(&StoredAssetsData)
	.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList);

	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView()
{
	AssetsDataToDeleteArray.Empty();
	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}

#pragma region RowWidgetForAssetListView

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay.IsValid()) return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);

	const FString DisplayAssetClassName = AssetDataToDisplay->AssetClassPath.GetAssetName().ToString();
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();

	FSlateFontInfo AssetClassNameFont = GetEmbossedTextFont();
	AssetClassNameFont.Size = 10.0f;

	FSlateFontInfo AssetNameFont = GetEmbossedTextFont();
	AssetNameFont.Size = 15.0f;

	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget = SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable).Padding(FMargin(5.0f))
	[
		SNew(SHorizontalBox)
		// First slot for check box
		
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.FillWidth(0.05f)
		[
			ConstructCheckbox(AssetDataToDisplay)
		]

		//Second slot for displaying asset class name

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.FillWidth(0.5f)
		[
			ConstructTextForRowWidget(DisplayAssetClassName, AssetClassNameFont)
		]

		// Third slot for displaying asset name

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		[
			ConstructTextForRowWidget(DisplayAssetName, AssetNameFont)
		]

		// Fourth slot for a button

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		[
			ConstructButtonForRowWidget(AssetDataToDisplay)
		]

	];

	return ListViewRowWidget;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		if (AssetsDataToDeleteArray.Contains(AssetData))
		{
			AssetsDataToDeleteArray.Remove(AssetData);
		}
		break;
	case ECheckBoxState::Checked:
		AssetsDataToDeleteArray.AddUnique(AssetData);
		break;
	case ECheckBoxState::Undetermined:
		break;
	default:
		break;
	}
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
	.Type(ESlateCheckBoxType::CheckBox)
	.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
	.Visibility(EVisibility::Visible);

	return ConstructedCheckBox;
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
	.Text(FText::FromString(TextContent))
	.Font(FontToUse)
	.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton> ConstructedButton = SNew(SButton)
	.Text(FText::FromString(TEXT("Delete")))
	.OnClicked(this, &SAdvanceDeletionTab::OnClickDeleteButton, AssetDataToDisplay);

	return ConstructedButton;
}

FReply SAdvanceDeletionTab::OnClickDeleteButton(TSharedPtr<FAssetData> ClickedAssetData)
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	const bool bResult = SuperManagerModule.DeleteSingleAssetForAssetList(*ClickedAssetData.Get());

	if (bResult)
	{
		// Updating the list source item
		if (StoredAssetsData.Contains(ClickedAssetData))
		{
			StoredAssetsData.Remove(ClickedAssetData);
		}

		// Refresh the list 
		RefreshAssetListView();
	}

	return FReply::Handled();
}

#pragma endregion

#pragma region TabButtons

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteAllButton()
{
	TSharedRef<SButton> DeleteAllButton = SNew(SButton)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.OnClicked(this, &SAdvanceDeletionTab::OnClickDeleteAllButton);

	DeleteAllButton->SetContent(ConstuctTabButtonsText(TEXT("Delete All")));

	return DeleteAllButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructSelectAllButton()
{
	TSharedRef<SButton> SelectAllButton = SNew(SButton)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.OnClicked(this, &SAdvanceDeletionTab::OnClickSelectAllButton);

	SelectAllButton->SetContent(ConstuctTabButtonsText(TEXT("Select All")));

	return SelectAllButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructUnSelectAllButton()
{
	TSharedRef<SButton> UnSelectAllButton = SNew(SButton)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.OnClicked(this, &SAdvanceDeletionTab::OnClickUnSelectAllButton);

	UnSelectAllButton->SetContent(ConstuctTabButtonsText(TEXT("UnSelect All")));

	return UnSelectAllButton;
}

FReply SAdvanceDeletionTab::OnClickDeleteAllButton()
{
	if (AssetsDataToDeleteArray.Num() == 0)
	{
		// None
		return FReply::Handled();
	}

	TArray<FAssetData> AssetDataToDelete;
	AssetDataToDelete.Reserve(AssetsDataToDeleteArray.Num());

	for (const TSharedPtr<FAssetData>& Data : AssetsDataToDeleteArray)
	{
		AssetDataToDelete.Add(*Data.Get());
	}

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	const bool bIsDeleted = SuperManagerModule.DeleteMultipleAssetForAssetList(AssetDataToDelete);

	if (bIsDeleted)
	{
		for (const TSharedPtr<FAssetData>& DeletedData : AssetsDataToDeleteArray)
		{
			if (StoredAssetsData.Contains(DeletedData))
			{
				StoredAssetsData.Remove(DeletedData);
			}
		}

		RefreshAssetListView();
	}

	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnClickSelectAllButton()
{
	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnClickUnSelectAllButton()
{
	return FReply::Handled();
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstuctTabButtonsText(const FString& TextContent)
{
	FSlateFontInfo ButtonTextFont = GetEmbossedTextFont();
	ButtonTextFont.Size = 15.0f;

	TSharedRef<STextBlock> TextBlock = SNew(STextBlock)
	.Text(FText::FromString(TextContent))
	.Font(ButtonTextFont)
	.Justification(ETextJustify::Center);

	return TextBlock;
}

#pragma endregion
