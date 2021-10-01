// Copyright Soren Gilbertson & Brady Herzog

using UnrealBuildTool;
using System.Collections.Generic;

public class GameDemoTarget : TargetRules
{
	public GameDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GameDemo" } );
	}
}
