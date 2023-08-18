// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FTurnBasedRPGCombatEditorModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	IConsoleCommand* CreateSimpleTestFilesCommand = nullptr;
	IConsoleCommand* CreateAutomationSpecFilesCommand = nullptr;
	IConsoleCommand* RefreshProjectCodeFilesCommand = nullptr;

	TSharedPtr<FExtender> ToolbarExtender;
	TSharedPtr<const FExtensionBase> Extension;
	TSharedPtr<FUICommandList> UICommandList;

	void RefreshProject();
	void CreateSimpleTestFiles(const TArray<FString>& Args);
	void CreateAutomationSpecFiles(const TArray<FString>& Args);
	TSharedRef<ITableRow> GenerateItemRow(TSharedPtr<FText> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OpenAlignmentEditor();
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void RegisterMenus();

	void UnregisterCommand(IConsoleCommand*& Command);
};


