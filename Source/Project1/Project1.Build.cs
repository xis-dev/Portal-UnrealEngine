// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project1 : ModuleRules
{
	public Project1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

	PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		PublicIncludePaths.AddRange(new string[] {
			"Project1",
			"Project1/Variant_Horror",
			"Project1/Variant_Horror/UI",
			"Project1/Variant_Shooter",
			"Project1/Variant_Shooter/AI",
			"Project1/Variant_Shooter/UI",
			"Project1/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
