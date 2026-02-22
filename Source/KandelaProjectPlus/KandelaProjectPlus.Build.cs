// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KandelaProjectPlus : ModuleRules
{
	public KandelaProjectPlus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"GameplayAbilities", "GameplayTasks", "GameplayTags"});

		PublicIncludePaths.AddRange(new string[]
		{
			ModuleDirectory + "/Public",
			ModuleDirectory + "/Public/GAS",
			ModuleDirectory + "/Public/GAS/Abilities",
			ModuleDirectory + "/Public/GAS/Core",
			ModuleDirectory + "/Public/GAS/Effects"
		});

		PrivateIncludePaths.AddRange(new string[]
		{
			ModuleDirectory + "/Private",
			ModuleDirectory + "/Private/GAS",
			ModuleDirectory + "/Private/GAS/Abilities",
			ModuleDirectory + "/Private/GAS/Core",
			ModuleDirectory + "/Private/GAS/Effects"
		});
	}
}
