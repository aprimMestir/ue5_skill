#include "Abilities/SkillAbilitySystemComponent.h"
#include "Abilities/SkillGameplayAbility.h"
#include "GameplayAbilitySpec.h"

bool USkillAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTag)
{
    if (!AbilityTag.IsValid())
    {
        return false;
    }

    for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities().Items)
    {
        const USkillGameplayAbility* SkillAbility = Cast<USkillGameplayAbility>(Spec.Ability);
        if (SkillAbility && SkillAbility->AbilityTag == AbilityTag)
        {
            return TryActivateAbility(Spec.Handle);
        }
    }

    return false;
}
