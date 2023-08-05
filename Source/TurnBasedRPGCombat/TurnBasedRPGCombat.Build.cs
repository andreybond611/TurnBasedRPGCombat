// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TurnBasedRPGCombat : ModuleRules
{
	public TurnBasedRPGCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem", "Slate", "SlateCore", "UMG",
			"GameplayAbilities", "GameplayTags", "Niagara"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
