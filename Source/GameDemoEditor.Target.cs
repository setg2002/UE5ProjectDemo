// Copyright Soren Gilbertson & Brady Herzog

using UnrealBuildTool;
using System.Collections.Generic;

public class GameDemoEditorTarget : TargetRules
{
	public GameDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GameDemo" } );
	}
}
