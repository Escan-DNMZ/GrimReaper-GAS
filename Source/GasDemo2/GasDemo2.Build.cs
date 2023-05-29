// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GasDemo2 : ModuleRules
{
	public GasDemo2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" ,"NavigationSystem","HeadMountedDisplay"});
        PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
    }
}
