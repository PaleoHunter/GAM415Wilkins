// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAM415FirstPerson : ModuleRules
{
	public GAM415FirstPerson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "ProceduralMeshComponent" });
	}
}
