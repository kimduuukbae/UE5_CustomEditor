#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"

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
		.AutoHeight()
		[
			SNew(SScrollBox)

			+SScrollBox::Slot()
			[
				SNew(SListView<TSharedPtr<FAssetData>>)
				.ItemHeight(24.0f)
				.ListItemsSource(&StoredAssetsData)
				.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
			]
		]

		// Fourth Slot
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]

	];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();


	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget = SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
	[
		SNew(STextBlock)
		.Text(FText::FromString(DisplayAssetName))
	];

	return ListViewRowWidget;
}
