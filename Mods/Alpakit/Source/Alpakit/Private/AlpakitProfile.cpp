﻿#include "AlpakitProfile.h"
#include "PlatformInfo.h"

FString FAlpakitProfile::MakeUATPlatformArgs() {
	// Code below replicates the minimum configuration required from FLauncherWorker::CreateUATCommand
	FString CommandLine;
	
	FString ServerCommand = TEXT("");
	FString ServerPlatforms = TEXT("");
	FString Platforms = TEXT("");
	FString PlatformCommand = TEXT("");
	FString OptionalParams = TEXT("");
	TSet<FString> OptionalTargetPlatforms;
	
	for (const FString& PlatformName : CookedPlatforms)
	{
		// Platform info for the given platform
		const PlatformInfo::FTargetPlatformInfo* PlatformInfo = PlatformInfo::FindPlatformInfo(FName(*PlatformName));

		if (ensure(PlatformInfo))
		{
			// separate out Server platforms
			FString& PlatformString = (PlatformInfo->PlatformType == EBuildTargetType::Server) ? ServerPlatforms : Platforms;

			PlatformString += TEXT("+");
			PlatformString += PlatformInfo->DataDrivenPlatformInfo->UBTPlatformString;

			// Append any extra UAT flags specified for this platform flavor
			if (!PlatformInfo->UATCommandLine.IsEmpty())
			{
				FString OptionalUATCommandLine = PlatformInfo->UATCommandLine;

				OptionalParams += TEXT(" ");
				OptionalParams += OptionalUATCommandLine;
			}
		}
	}

	// If both Client and Server are desired to be built avoid Server causing clients to not be built PlatformInfo wise
	if (OptionalParams.Contains(TEXT("-client")) && OptionalParams.Contains(TEXT("-noclient")))
	{
		OptionalParams = OptionalParams.Replace(TEXT("-noclient"), TEXT(""));
	}

	if (ServerPlatforms.Len() > 0)
	{
		ServerCommand = TEXT(" -server -serverplatform=") + ServerPlatforms.RightChop(1);
		if (Platforms.Len() == 0)
		{
			OptionalParams += TEXT(" -noclient");
		}
	}
	if (Platforms.Len() > 0)
	{
		PlatformCommand = TEXT(" -platform=") + Platforms.RightChop(1);
	}
	
	CommandLine += PlatformCommand;
	CommandLine += ServerCommand;
	CommandLine += OptionalParams;

	return CommandLine;
}

FString FAlpakitProfile::MakeUATCommandLine() {
	const FString ProjectPath = FPaths::IsProjectFilePathSet()
		? FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath())
		: FPaths::RootDir() / FApp::GetProjectName() / FApp::GetProjectName() + TEXT(".uproject");
	
	FString CommandLine = FString::Printf(TEXT("-ScriptsForProject=\"%s\" PackagePlugin -project=\"%s\" -clientconfig=%s -serverconfig=%s -utf8output -DLCName=%s"),
			*ProjectPath,
			*ProjectPath,
			LexToString(BuildConfiguration),
			LexToString(BuildConfiguration),
			*PluginName);

	if (bBuild) {
		CommandLine += TEXT(" -build");
	}

	CommandLine += MakeUATPlatformArgs();

	for (auto [Platform, GameInfo] : PlatformGameInfo) {
		if (GameInfo.bCopyToGame) {
			CommandLine += FString::Printf(TEXT(" -CopyToGameDirectory_%s=\"%s\""), *Platform, *GameInfo.GamePath);
		}
		if (GameInfo.StartGameType != EAlpakitStartGameType::NONE) {
			CommandLine += FString::Printf(TEXT(" -LaunchGame_%s=%s"), *Platform, LexToString(GameInfo.StartGameType));
		}
	}

	return CommandLine;
}
