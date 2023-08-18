// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved
#pragma once

#include "Framework/Commands/Commands.h"
#include "AlignmentEditorWindow/AlignmentEditorButtonStyle.h"

class FRPGEditorCommands : public TCommands<FRPGEditorCommands>
{
public:
	FRPGEditorCommands()
		: TCommands(TEXT("AlignmentEditor"), NSLOCTEXT("Contexts", "AlignmentEditor", "RPG Editor Commands"), NAME_None,
					FAlignmentEditorButtonStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> AlignmentEditorButton;
};
