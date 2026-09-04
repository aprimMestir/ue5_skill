#pragma once

#include "CoreMinimal.h"
#include "Abilities/SkillGameplayAbility.h"
#include "TimerManager.h"
#include "SkillDashAbility.generated.h"

class UAbilitySystemComponent;

UCLASS(Blueprintable)
class SKILL_API USkillDashAbility : public USkillGameplayAbility
{
    GENERATED_BODY()

public:
    USkillDashAbility();

    virtual bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags = nullptr,
        const FGameplayTagContainer* TargetTags = nullptr,
        FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash", meta=(ClampMin="0.0"))
    float DashStrength = 1200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash", meta=(ClampMin="0.0"))
    float VerticalBoost = 80.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash", meta=(ClampMin="0.0"))
    float StaminaCost = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash", meta=(ClampMin="0.0"))
    float CooldownDuration = 1.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash")
    FGameplayTag CooldownTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash")
    FGameplayTag BlockedStateTag;

private:
    void ClearCooldownTag();

    TWeakObjectPtr<UAbilitySystemComponent> CooldownAbilitySystem;
    FTimerHandle CooldownTimerHandle;
};
