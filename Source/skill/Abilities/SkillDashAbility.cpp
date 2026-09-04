#include "Abilities/SkillDashAbility.h"
#include "Abilities/SkillAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

USkillDashAbility::USkillDashAbility()
{
    DisplayName = FText::FromString(TEXT("Dash"));
    Description = FText::FromString(TEXT("Quickly dash in the current movement direction."));

    AbilityTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Movement.Dash")), false);
    CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Cooldown.Dash")), false);
    BlockedStateTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.Stunned")), false);
}

bool USkillDashAbility::CanActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        return false;
    }

    const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

    if (CooldownTag.IsValid() && ASC->HasMatchingGameplayTag(CooldownTag))
    {
        return false;
    }

    if (BlockedStateTag.IsValid() && ASC->HasMatchingGameplayTag(BlockedStateTag))
    {
        return false;
    }

    const float CurrentStamina = ASC->GetNumericAttribute(USkillAttributeSet::GetStaminaAttribute());
    return CurrentStamina >= StaminaCost;
}

void USkillDashAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    ASC->ApplyModToAttribute(USkillAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -StaminaCost);

    FVector DashDirection = Character->GetLastMovementInputVector();
    DashDirection.Z = 0.0f;

    if (DashDirection.IsNearlyZero())
    {
        DashDirection = Character->GetActorForwardVector();
        DashDirection.Z = 0.0f;
    }

    DashDirection.Normalize();

    const FVector LaunchVelocity = DashDirection * DashStrength + FVector::UpVector * VerticalBoost;
    Character->LaunchCharacter(LaunchVelocity, true, true);

    if (CooldownTag.IsValid() && CooldownDuration > 0.0f)
    {
        ASC->AddLooseGameplayTag(CooldownTag);
        CooldownAbilitySystem = ASC;

        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(CooldownTimerHandle);
            World->GetTimerManager().SetTimer(
                CooldownTimerHandle,
                this,
                &USkillDashAbility::ClearCooldownTag,
                CooldownDuration,
                false);
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void USkillDashAbility::ClearCooldownTag()
{
    if (CooldownAbilitySystem.IsValid() && CooldownTag.IsValid())
    {
        CooldownAbilitySystem->RemoveLooseGameplayTag(CooldownTag);
    }

    CooldownAbilitySystem.Reset();
}
