// Copyright Soatori Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MxfGame : ModuleRules
{
	public MxfGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"MxfGame"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"GameplayTags",
				"GameplayTasks",
				"GameplayAbilities",
				"ModularGameplay",
				"ModularGameplayActors"
				}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"NetCore",
				"GameplayMessageRuntime"
				}
			);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
