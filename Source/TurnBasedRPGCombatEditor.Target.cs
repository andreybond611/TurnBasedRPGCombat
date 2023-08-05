// Copyright 2022 Andrei Bondarenko. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class TurnBasedRPGCombatEditorTarget : TargetRules
{
	public TurnBasedRPGCombatEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "TurnBasedRPGCombatEditor" } );
	}
}
