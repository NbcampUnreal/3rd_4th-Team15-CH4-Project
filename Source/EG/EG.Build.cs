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
		
         string PhotonRoot = Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "Photon");

        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(PhotonRoot),
            Path.Combine(PhotonRoot, "Common-cpp", "inc"),
            Path.Combine(PhotonRoot, "Photon-cpp", "inc"),
            Path.Combine(PhotonRoot, "LoadBalancing-cpp", "inc"),
        });

        string CommonLibDir = Path.Combine(PhotonRoot, "Common-cpp", "lib");
        string PhotonLibDir = Path.Combine(PhotonRoot, "Photon-cpp", "lib");
        string LoadBalancingLibDir = Path.Combine(PhotonRoot, "LoadBalancing-cpp", "lib");
        

        // Debug만 진짜 디버그 라이브러리 사용. DebugGame/Development/Shipping은 release 사용.
        bool useDebugLib = Target.Configuration == UnrealTargetConfiguration.Debug;

        string vcVer = "vc17"; // VS2022 기준. 만약 폴더에 vc16만 있으면 "vc16"으로 변경.
        string crt = "md"; // 보통 md 권장. mt만 있다면 "mt"로 변경.

        string cfg = useDebugLib ? "debug" : "release";

        string arch = "x64"; // UE 타깃 아키텍처 (Win64 빌드 시)
        string patternCommon = $"Common-cpp_{vcVer}_{cfg}_windows_{crt}_{arch}.lib";
        string patternPhoton = $"Photon-cpp_{vcVer}_{cfg}_windows_{crt}_{arch}.lib";
        string patternLoadBalancing = $"LoadBalancing-cpp_{vcVer}_{cfg}_windows_{crt}_{arch}.lib";


        string PickLib(string dir, string pattern, string name)
        {
            if (!Directory.Exists(dir))
                throw new System.Exception($"라이브러리 폴더 없음: {dir}");
            var files = Directory.GetFiles(dir, pattern);
            if (files.Length == 0)
                throw new System.Exception($"{name} 라이브러리 미발견: {dir} / 패턴 {pattern}");
            return files.OrderByDescending(f => f.Length).First();
        }

        PublicAdditionalLibraries.Add(PickLib(CommonLibDir, patternCommon, "Common-cpp"));
        PublicAdditionalLibraries.Add(PickLib(PhotonLibDir, patternPhoton, "Photon-cpp"));
        PublicAdditionalLibraries.Add(PickLib(LoadBalancingLibDir, patternLoadBalancing, "LoadBalancing-cpp"));

        bEnableExceptions = true;
        bUseRTTI = true;
    
		
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
