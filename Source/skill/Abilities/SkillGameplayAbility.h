#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SkillGameplayAbility.generated.h"

class UTexture2D;

UCLASS(Abstract, Blueprintable)
class SKILL_API USkillGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    USkillGameplayAbility();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(MultiLine=true))
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    TObjectPtr<UTexture2D> Icon;
};
