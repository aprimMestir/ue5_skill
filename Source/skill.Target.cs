using UnrealBuildTool;
using System.Collections.Generic;

public class skillTarget : TargetRules
{
    public skillTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "skill" });
    }
}
