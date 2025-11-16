// Copyright Soatori Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MxfEditorTarget : TargetRules
{
	public MxfEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MxfGame" } );
	}
}
