// Copyright Epic Games, Inc. All Rights Reserved.

#include "Engine/Selection.h"
#include "LevelEditor.h"
#include "ReverseHullToolkit.h"
#include "ReverseHullToolkitStyle.h"
#include "ReverseHullToolkitCommands.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName ReverseHullToolkitTabName("ReverseHullToolkit");

#define LOCTEXT_NAMESPACE "FReverseHullToolkitModule"

void FReverseHullToolkitModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FReverseHullToolkitStyle::Initialize();
	FReverseHullToolkitStyle::ReloadTextures();

	FReverseHullToolkitCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FReverseHullToolkitCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FReverseHullToolkitModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FReverseHullToolkitModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ReverseHullToolkitTabName, FOnSpawnTab::CreateRaw(this, &FReverseHullToolkitModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FReverseHullToolkitTabTitle", "ReverseHullToolkit"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	

}

void FReverseHullToolkitModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FReverseHullToolkitStyle::Shutdown();

	FReverseHullToolkitCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ReverseHullToolkitTabName);
}

TSharedRef<SDockTab> FReverseHullToolkitModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	HullGenerator = MakeShared<ReverseHullGenerator>();

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FReverseHullToolkitModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("ReverseHullToolkit.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.AutoHeight()
			.Padding(10, 10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("WidgetText", "Select the actors to which you want to generate the outline"))
			]
			+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.AutoHeight()
				.Padding(10, 10)
			[
				SNew(SButton)
				.Text(LOCTEXT("Generate Outline", "Generate Outline"))
			.OnClicked(FOnClicked::CreateRaw(this, &FReverseHullToolkitModule::OnGenerateButtonClicked))
			]
			+ SVerticalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.AutoHeight()
			.Padding(10, 10)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10, 10)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("DoCompactMesh Text", "Compact mesh"))
				.ToolTipText(LOCTEXT("DoCompactMesh Text Tooltip", "Currently not implemented"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10, 10)
				[
					SNew(SCheckBox)
					.ToolTipText(LOCTEXT("DoCompactMesh", "Currently not implemented"))
					.OnCheckStateChanged(FOnCheckStateChanged::CreateLambda([this](ECheckBoxState CheckboxState)
					{
						switch (CheckboxState)
						{
						case ECheckBoxState::Unchecked:
							DoCompactMesh = false;
							break;
						case ECheckBoxState::Checked:
							DoCompactMesh = true;
							break;
						case ECheckBoxState::Undetermined:
							break;
						default:
							DoCompactMesh = false;
							break;
						}
					}))
				]
			]
		];
}

FReply FReverseHullToolkitModule::OnGenerateButtonClicked()
{
	// Do generation logic here
	TArray<AActor*> actors;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(actors);

	for (AActor* actor : actors)
	{
		if (HullGenerator->GenerateHullMesh(actor, DoCompactMesh))
		{
			UE_LOG(LogTemp, Display, TEXT("Hull Generated"))
		}
	}
	
	return FReply::Handled();
}

void FReverseHullToolkitModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ReverseHullToolkitTabName);
}

void FReverseHullToolkitModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("ToolsToolsRhs");
			Section.AddMenuEntryWithCommandList(FReverseHullToolkitCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FReverseHullToolkitCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FReverseHullToolkitModule, ReverseHullToolkit)