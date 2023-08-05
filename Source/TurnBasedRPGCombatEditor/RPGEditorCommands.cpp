// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "RPGEditorCommands.h"
#include "TurnBasedRPGCombatEditor.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "CustomEditorUI"

void FRPGEditorCommands::RegisterCommands()
{
	UI_COMMAND(AlignmentEditorButton, "Alignment Editor", "Open Alignment Editor", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
