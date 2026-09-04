#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "SkillAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Abilities), meta=(BlueprintSpawnableComponent))
class SKILL_API USkillAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Abilities")
    bool TryActivateAbilityByTag(FGameplayTag AbilityTag);
};
