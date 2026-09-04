#include "Characters/SkillCharacterBase.h"
#include "Abilities/SkillAbilitySystemComponent.h"
#include "Abilities/SkillAttributeSet.h"
#include "Abilities/SkillGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

ASkillCharacterBase::ASkillCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<USkillAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<USkillAttributeSet>(TEXT("AttributeSet"));
}

void ASkillCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    InitializeAbilityActorInfo();
}

UAbilitySystemComponent* ASkillCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ASkillCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitializeAbilityActorInfo();
    GrantStartupAbilities();
}

void ASkillCharacterBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitializeAbilityActorInfo();
}

void ASkillCharacterBase::InitializeAbilityActorInfo()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

void ASkillCharacterBase::GrantStartupAbilities()
{
    if (!HasAuthority() || !AbilitySystemComponent || bStartupAbilitiesGranted)
    {
        return;
    }

    for (const TSubclassOf<USkillGameplayAbility>& AbilityClass : StartupAbilities)
    {
        if (AbilityClass)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
        }
    }

    bStartupAbilitiesGranted = true;
}
