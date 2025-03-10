#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "SuperManager.h"
#include "EditorAssetLibrary.h"
#include "AssetRegistryModule.h"
#include "Widgets/Input/SSearchBox.h"
#include "DebugHeader.h"

#define ListAllText TEXT("List All Available Assets")
#define ListUnusedText TEXT("List Unused Assets")

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;

	FSlateFontInfo ComboBoxHelpTextFont = GetEmbossedTextFont();
	ComboBoxHelpTextFont.Size = 15;

	StoredAssetsData = InArgs._AssetsDataToStore;
	DisplayedAssetsData = StoredAssetsData;

	CheckBoxesArray.Empty();
	AssetsDataToDeleteArray.Empty();
	ComboSourceItems.Empty();

	ComboSourceItems.Add(MakeShared<FString>(ListAllText));
	ComboSourceItems.Add(MakeShared<FString>(ListUnusedText));

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
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				ConstructComboBox()
			]

			+ SHorizontalBox::Slot()
			[
				ConstructSearchBox()
			]

			+ SHorizontalBox::Slot()
			.FillWidth(0.1f)
			[
				ConstructComboHelpText(TEXT("Current Folder : \n") + InArgs._CurrentSelectedFolder, ComboBoxHelpTextFont)
			]
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
	.ListItemsSource(&DisplayedAssetsData)
	.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
	.OnMouseButtonDoubleClick(this, &SAdvanceDeletionTab::OnDoubleClickRowWidget);

	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView()
{
	AssetsDataToDeleteArray.Empty();
	CheckBoxesArray.Empty();

	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}

TSharedRef<SSearchBox> SAdvanceDeletionTab::ConstructSearchBox()
{
	SAssignNew(SearchBox, SSearchBox)
		.HintText(FText::FromString("Enter text to Asset Name"))
		.OnTextChanged_Lambda([this](const FText& Text)
		{
			if (Text.IsEmpty())
			{
				DebugHeader::Print(TEXT("Text Is Empty"), FColor::Cyan);
				DisplayedAssetsData = StoredAssetsData;
			}
			else
			{
				DisplayedAssetsData.Empty();
				for (const TSharedPtr<FAssetData>& AssetData : StoredAssetsData)
				{
					if (AssetData->AssetName.ToString().Contains(Text.ToString()))
					{
						DisplayedAssetsData.Add(AssetData);
					}
				}
			}

			RefreshAssetListView();
		});


	return SearchBox.ToSharedRef();
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

	TArray<FName> ReferencersName;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	AssetRegistryModule.Get().GetReferencers(AssetDataToDisplay->PackageName, ReferencersName);

	int32 ReferenceCount = ReferencersName.Num();

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

		// Reference Count Text
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			ConstructTextForRowWidget(
				TEXT("Reference Count : ") + FString::FromInt(ReferenceCount),
				AssetNameFont
			)
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

	CheckBoxesArray.Add(ConstructedCheckBox);

	return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnDoubleClickRowWidget(TSharedPtr<FAssetData> ClickedData)
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	SuperManagerModule.SyncContentBrowser(ClickedData->ObjectPath.ToString());
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

		if (DisplayedAssetsData.Contains(ClickedAssetData))
		{
			DisplayedAssetsData.Remove(ClickedAssetData);
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

			if (DisplayedAssetsData.Contains(DeletedData))
			{
				DisplayedAssetsData.Remove(DeletedData);
			}
		}

		

		RefreshAssetListView();
	}

	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnClickSelectAllButton()
{
	if (CheckBoxesArray.Num() == 0)
	{
		return FReply::Handled();
	}

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (!CheckBox->IsChecked())
		{
			CheckBox->ToggleCheckedState();
		}
	}

	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnClickUnSelectAllButton()
{
	if (CheckBoxesArray.Num() == 0)
	{
		return FReply::Handled();
	}

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (CheckBox->IsChecked())
		{
			CheckBox->ToggleCheckedState();
		}
	}

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

#pragma region ComboBox

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTab::ConstructComboBox()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox = SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboSourceItems)
		.OnGenerateWidget(this, &SAdvanceDeletionTab::OnGenerateComboContent)
		.OnSelectionChanged(this, &SAdvanceDeletionTab::OnComboSelectionChanged)
		[
			SAssignNew(ComboDisplayTextBlock, STextBlock)
			.Text(FText::FromString(TEXT("List Assets Option")))
		];

	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvanceDeletionTab::OnGenerateComboContent(TSharedPtr<FString> SourceItem)
{
	TSharedRef<STextBlock> ConstructedComboText = SNew(STextBlock).Text(FText::FromString(*SourceItem.Get()));
	return ConstructedComboText;
}

void SAdvanceDeletionTab::OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo)
{
	ComboDisplayTextBlock->SetText(FText::FromString(*SelectedOption.Get()));

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	//Pass data for our module to filter

	if (*SelectedOption.Get() == ListAllText)
	{
		// List all stored asset data
		DisplayedAssetsData = StoredAssetsData;
		RefreshAssetListView();
	}
	else if (*SelectedOption.Get() == ListUnusedText)
	{
		// List all unused assets
		DisplayedAssetsData.Empty();

		SuperManagerModule.ListUnusedAssetsForAssetList(StoredAssetsData, DisplayedAssetsData);
		RefreshAssetListView();
	}
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructComboHelpText(const FString& Text, const FSlateFontInfo& FontToUse)
{
	return SNew(STextBlock)
		.Text(FText::FromString(Text))
		.Font(FontToUse)
		.Justification(ETextJustify::Center)
		.AutoWrapText(true);
}

#pragma endregion