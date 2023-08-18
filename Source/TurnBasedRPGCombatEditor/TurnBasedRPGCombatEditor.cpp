// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "TurnBasedRPGCombatEditor.h"
#include "SourceCodeNavigation.h"
#include "Dialogs/SOutputLogDialog.h"
#include "GameProjectGeneration/Public/GameProjectGenerationModule.h"
#include "GeneralProjectSettings.h"
#include "RPGEditorCommands.h"
#include "AlignmentEditorWindow/AlignmentEditorButtonStyle.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

// clang-format off

static const FName TurnBasedRPGCombatEditorTabName("TurnBasedRPGCombatEditor");

#define LOCTEXT_NAMESPACE "MainFrameActions"

DEFINE_LOG_CATEGORY_STATIC(FLogTurnBasedModuleEditor, All, All)

IMPLEMENT_GAME_MODULE(FTurnBasedRPGCombatEditorModule, TurnBasedRPGCombatEditor)

void FTurnBasedRPGCombatEditorModule::StartupModule()
{
	RefreshProjectCodeFilesCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("RefreshProjectCodeFiles"),
		TEXT("Refreshes your C++ code project in Visual Studio"),
		FConsoleCommandDelegate::CreateRaw(this, &FTurnBasedRPGCombatEditorModule::RefreshProject), ECVF_Default);

	CreateSimpleTestFilesCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("CreateSimpleTestFiles"),
		TEXT("Creates .cpp and .h files for simple automation tests. Requires a name as the first parameter"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FTurnBasedRPGCombatEditorModule::CreateSimpleTestFiles), ECVF_Default);

	CreateAutomationSpecFilesCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("CreateAutomationSpecFiles"),
		TEXT("Creates .cpp and .h files for automation spec tests. Requires a name as the first parameter"),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FTurnBasedRPGCombatEditorModule::CreateAutomationSpecFiles), ECVF_Default);



	FAlignmentEditorButtonStyle::Initialize();
	FAlignmentEditorButtonStyle::ReloadTextures();

	FRPGEditorCommands::Register();

	UICommandList = MakeShareable(new FUICommandList());

	UICommandList->MapAction(FRPGEditorCommands::Get().AlignmentEditorButton,
						   FExecuteAction::CreateRaw(this, &FTurnBasedRPGCombatEditorModule::OpenAlignmentEditor),
						   FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTurnBasedRPGCombatEditorModule::RegisterMenus));
}

void FTurnBasedRPGCombatEditorModule::UnregisterCommand(IConsoleCommand*& Command)
{
	if (Command)
	{
		IConsoleManager::Get().UnregisterConsoleObject(Command);
		Command = nullptr;
	}
}

void FTurnBasedRPGCombatEditorModule::ShutdownModule()
{
	UnregisterCommand(CreateSimpleTestFilesCommand);
	UnregisterCommand(RefreshProjectCodeFilesCommand);
	UnregisterCommand(CreateAutomationSpecFilesCommand);

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FAlignmentEditorButtonStyle::Shutdown();
	FRPGEditorCommands::Unregister();
}

void FTurnBasedRPGCombatEditorModule::RefreshProject()
{
	if (!FSourceCodeNavigation::IsCompilerAvailable())
	{
		// Attempt to trigger the tutorial if the user doesn't have a compiler installed for the project.
		FSourceCodeNavigation::AccessOnCompilerNotFound().Broadcast();
	}

	FText FailReason, FailLog;
	if (!FGameProjectGenerationModule::Get().UpdateCodeProject(FailReason, FailLog))
	{
		SOutputLogDialog::Open(LOCTEXT("RefreshProject", "Refresh Project"), FailReason, FailLog, FText::GetEmpty());
	}
}

void FTurnBasedRPGCombatEditorModule::CreateSimpleTestFiles(const TArray<FString>& Args)
{
	const FString TestName = Args.IsValidIndex(0) ? Args[0] : "Untitled";
	const FString CopyrightNotice = GetDefault<UGeneralProjectSettings>()->CopyrightNotice;

	const FString SimpleTestCPPTemplateLocation = FPaths::ProjectDir().Append(TEXT("/CodeTemplates/SimpleTest.cpp.template"));
	const FString SimpleTestHeaderTemplateLocation = FPaths::ProjectDir().Append(TEXT("/CodeTemplates/SimpleTest.h.template"));

	const FString TestLocation = FString::Printf(TEXT("%s%s/Private/Tests/"), *FPaths::GameSourceDir(), FApp::GetProjectName());

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (FileManager.FileExists(*SimpleTestCPPTemplateLocation))
	{
		if (!FFileHelper::LoadFileToString(FileContent, *SimpleTestCPPTemplateLocation, FFileHelper::EHashOptions::None))
		{
			UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Expected file location: %s"), *SimpleTestCPPTemplateLocation);
	}

	FString TestFileDestination = TestLocation + TestName + "Test.cpp";
	if (FileManager.FileExists(*TestFileDestination))
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: File %s already exists"), *TestFileDestination);
		return;
	}
	FileContent = FileContent.Replace(TEXT("Copyright"), *CopyrightNotice);
	FileContent = FileContent.Replace(TEXT("TestName"), *TestName);

	FFileHelper::SaveStringToFile(FileContent, *TestFileDestination);

	if (FileManager.FileExists(*SimpleTestHeaderTemplateLocation))
	{
		if (!FFileHelper::LoadFileToString(FileContent, *SimpleTestHeaderTemplateLocation, FFileHelper::EHashOptions::None))
		{
			UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Expected file location: %s"), *SimpleTestCPPTemplateLocation);
	}

	TestFileDestination = TestLocation + TestName + "Test.h";
	if (FileManager.FileExists(*TestFileDestination))
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: File %s already exists"), *TestFileDestination);
		return;
	}
	FileContent = FileContent.Replace(TEXT("Copyright"), *CopyrightNotice);

	FFileHelper::SaveStringToFile(FileContent, *TestFileDestination);

	RefreshProject();
}

void FTurnBasedRPGCombatEditorModule::CreateAutomationSpecFiles(const TArray<FString>& Args)
{
	const FString TestName = Args.IsValidIndex(0) ? Args[0] : "Untitled";
	const FString CopyrightNotice = GetDefault<UGeneralProjectSettings>()->CopyrightNotice;

	const FString AutomationSpecCPPTemplateLocation = FPaths::ProjectDir().Append(TEXT("/CodeTemplates/AutomationSpec.cpp.template"));
	const FString AutomationSpecHeaderTemplateLocation = FPaths::ProjectDir().Append(TEXT("/CodeTemplates/AutomationSpec.h.template"));

	const FString TestLocation = FString::Printf(TEXT("%s%s/Private/Tests/"), *FPaths::GameSourceDir(), FApp::GetProjectName());

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (FileManager.FileExists(*AutomationSpecCPPTemplateLocation))
	{
		if (!FFileHelper::LoadFileToString(FileContent, *AutomationSpecCPPTemplateLocation, FFileHelper::EHashOptions::None))
		{
			UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Expected file location: %s"), *AutomationSpecCPPTemplateLocation);
	}

	FString TestFileDestination = TestLocation + TestName + ".spec.cpp";
	if (FileManager.FileExists(*TestFileDestination))
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: File %s already exists"), *TestFileDestination);
		return;
	}
	FileContent = FileContent.Replace(TEXT("Copyright"), *CopyrightNotice);
	FileContent = FileContent.Replace(TEXT("TestName"), *TestName);
	FileContent = FileContent.Replace(TEXT("ProjectName"), FApp::GetProjectName());

	FFileHelper::SaveStringToFile(FileContent, *TestFileDestination);

	if (FileManager.FileExists(*AutomationSpecHeaderTemplateLocation))
	{
		if (!FFileHelper::LoadFileToString(FileContent, *AutomationSpecHeaderTemplateLocation, FFileHelper::EHashOptions::None))
		{
			UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: Expected file location: %s"), *AutomationSpecCPPTemplateLocation);
	}

	TestFileDestination = TestLocation + TestName + ".spec.h";
	if (FileManager.FileExists(*TestFileDestination))
	{
		UE_LOG(FLogTurnBasedModuleEditor, Warning, TEXT("FileManipulation: File %s already exists"), *TestFileDestination);
		return;
	}
	FileContent = FileContent.Replace(TEXT("Copyright"), *CopyrightNotice);

	FFileHelper::SaveStringToFile(FileContent, *TestFileDestination);

	RefreshProject();
}

TSharedRef<ITableRow> FTurnBasedRPGCombatEditorModule::GenerateItemRow(TSharedPtr<FText> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FText>>, OwnerTable)
		[
			SNew(STextBlock)
			.Text(*Item)
		];
	
}

void FTurnBasedRPGCombatEditorModule::OpenAlignmentEditor()
{
	//TArray<TSharedPtr<FText>> Items;
	//Items.Add(MakeShareable(new FText(FText::FromString("Hello"))));
	//Items.Add(MakeShareable(new FText(FText::FromString("World"))));
	//Items.Add(MakeShareable(new FText(FText::FromString("Bye"))));


	//TSharedRef<SWindow> AlignmentEditorWindow = SNew(SWindow)
	//	.Title(NSLOCTEXT("CustomEditorUI", "AlignmentEditorWindowTitle", "Alignment Editor"))
	//	.ClientSize({800.f, 600.f})
	//	.SupportsMaximize(false)
	//	.SupportsMinimize(false)
	//	.SizingRule(ESizingRule::FixedSize)
	//	[
	//		SNew(SListView<TSharedPtr<FText>>)
	//		.ItemHeight(24)
	//		.ListItemsSource(&Items)
	//		.OnGenerateRow(this, &FTurnBasedRPGCombatEditorModule::GenerateItemRow)
	//		/*.OnContextMenuOpening(this, &FTurnBasedRPGCombatEditorModule::GetListContextMenu)
	//		.SelectionMode(this, &FTurnBasedRPGCombatEditorModule::GetSelectionMode)*/
	//		/*.HeaderRow
	//		(
	//			SNew(SHeaderRow)
	//			+ SHeaderRow::Column("Name")
	//			[
	//				SNew(SBorder)
	//				.Padding(5)
	//				.Content()
	//				[
	//					SNew(STextBlock)
	//					.Text(TEXT("Name"))
	//				]
	//			]
	//			+ SHeaderRow::Column("Number").DefaultLabel(TEXT("Number"))
	//			+ SHeaderRow::Column("TextField").DefaultLabel(TEXT("Text Field"))
	//			+ SHeaderRow::Column("TextBlock").DefaultLabel(TEXT("Text Block"))
	//			+ SHeaderRow::Column("AddChild").DefaultLabel(TEXT("Add Child"))
	//		)*/
	//	];

	//IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	//if (MainFrameModule.GetParentWindow().IsValid())
	//{
	//	FSlateApplication::Get().AddWindowAsNativeChild(AlignmentEditorWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	//}
	//else
	//{
	//	FSlateApplication::Get().AddWindow(AlignmentEditorWindow);
	//}

}

void FTurnBasedRPGCombatEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	const FSlateIcon IconBrush = FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.RepeatLastLaunch");

	Builder.AddToolBarButton(FRPGEditorCommands::Get().AlignmentEditorButton, NAME_None, 
							 NSLOCTEXT("CustomEditorUI", "AlignmentEditorButton", "Alignment Editor"),
							 NSLOCTEXT("CustomEditorUI", "AlignmentEditorTooltip", "Opens Alignment Editor"),
							 IconBrush, NAME_None);
}

void FTurnBasedRPGCombatEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRPGEditorCommands::Get().AlignmentEditorButton, UICommandList);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("CustomEditorTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRPGEditorCommands::Get().AlignmentEditorButton));
				Entry.SetCommandList(UICommandList);
			}
		}
	}
}

// clang-format on
