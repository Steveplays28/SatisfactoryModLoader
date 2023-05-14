// Fill out your copyright notice in the Description page of Project Settings.

using System.Collections.Generic;
using UnrealBuildTool;

public class FactoryGameEditorTarget : TargetRules
{
	public FactoryGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		CppStandard = CppStandardVersion.Cpp17;
		ExtraModuleNames.AddRange(new[] { "FactoryGame", "SML", "FicsitChat" });
		DefaultBuildSettings = BuildSettingsVersion.V2;
	}
}
