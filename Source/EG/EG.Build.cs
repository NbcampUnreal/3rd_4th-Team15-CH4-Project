// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Linq;
using System.IO;
public class EG : ModuleRules
{
	public EG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Initial Dependencies
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			
			// AI
			"AIModule", "NavigationSystem",
			
			// GAS
			"GameplayAbilities", "GameplayTags", "GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] { "EG" });

        PublicIncludePaths.AddRange(new string[] { "EG" });
		
         
    }
}
