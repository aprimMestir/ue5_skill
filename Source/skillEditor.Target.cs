using UnrealBuildTool;
using System.Collections.Generic;

public class skillEditorTarget : TargetRules
{
    public skillEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "skill" });
    }
}
