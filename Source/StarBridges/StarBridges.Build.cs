// Copyright Epic Games, Inc. All Rights Reserved.

using Tools.DotNETCommon;
using UnrealBuildTool;

public class StarBridges : ModuleRules
{
	public StarBridges(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Json", "JsonUtilities", "ProceduralMeshComponent", "Http", "UMG", "Slate", "SlateCore" });
	}
}
