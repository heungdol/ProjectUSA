// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectUSA : ModuleRules
{
	public ProjectUSA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange (new string[] {"ProjectUSA"});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG" , "LevelSequence", 
"MovieScene", "AIModule", "Niagara", "PhysicsCore"});

        PublicDependencyModuleNames.AddRange(new string[] {
            "GameFeatures",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

        // Window32 기능 사용하기 위함
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add("user32.lib");
        }

    }
}
