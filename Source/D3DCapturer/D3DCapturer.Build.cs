// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class D3DCapturer : ModuleRules
{
	public D3DCapturer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "RenderCore",
                "RHI",
                "Projects",
                "D3D11RHI",
                "D3D12RHI",
                "Media",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //    PublicAdditionalLibraries.Add("d3d11.lib");
        //    PublicAdditionalLibraries.Add("d3d11on12.lib");
        //}


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
