// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UE2Demo : ModuleRules
{
	public UE2Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableUndefinedIdentifierWarnings = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "UMG", "OnlineSubsystemSteam",
			"OnlineSubsystem","Slate","CusSteamOnline"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
		PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "libcurl_a.lib"));

		PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "ws2_32.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "wldap32.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "crypt32.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "advapi32.lib"));
	}

	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty")); }
	}
}