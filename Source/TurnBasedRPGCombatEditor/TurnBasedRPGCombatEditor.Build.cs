// Copyright 2022 Andrei Bondarenko. All rights reserved

using UnrealBuildTool;

public class TurnBasedRPGCombatEditor : ModuleRules
{
	public TurnBasedRPGCombatEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "RHI", "RenderCore", "MainFrame", "AssetTools", "AppFramework", "PropertyEditor", "ToolMenus" });
		PublicDependencyModuleNames.Add("TurnBasedRPGCombat");

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Slate", "SlateCore", "EditorStyle", 
		"GraphEditor", "BlueprintGraph", "EngineSettings", "Projects" });
	}
}
