#include "Characters/SkillCharacterBase.h"
#include "Abilities/SkillAbilitySystemComponent.h"
#include "Abilities/SkillAttributeSet.h"
#include "Abilities/SkillGameplayAbility.h"
#include "Abilities/SkillDashAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

ASkillCharacterBase::ASkillCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<USkillAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<USkillAttributeSet>(TEXT("AttributeSet"));

    DashKey = EKeys::LeftShift;
    DashAbilityTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Movement.Dash")), false);
    StartupAbilities.Add(USkillDashAbility::StaticClass());
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

void ASkillCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    ConfigureRuntimeAbilityInput(PlayerInputComponent);
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

bool ASkillCharacterBase::ActivateAbilityByTag(FGameplayTag AbilityTag)
{
    return AbilitySystemComponent && AbilitySystemComponent->TryActivateAbilityByTag(AbilityTag);
}

void ASkillCharacterBase::ConfigureRuntimeAbilityInput(UInputComponent* PlayerInputComponent)
{
    if (!bEnableRuntimeDashInput || !PlayerInputComponent)
    {
        return;
    }

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;

    if (!EnhancedInputComponent || !LocalPlayer)
    {
        return;
    }

    if (!RuntimeDashInputAction)
    {
        RuntimeDashInputAction = NewObject<UInputAction>(this, TEXT("IA_RuntimeDash"));
        RuntimeDashInputAction->ValueType = EInputActionValueType::Boolean;
    }

    if (!RuntimeAbilityMappingContext)
    {
        RuntimeAbilityMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_RuntimeAbilities"));
        RuntimeAbilityMappingContext->MapKey(RuntimeDashInputAction, DashKey);
    }

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        InputSubsystem->RemoveMappingContext(RuntimeAbilityMappingContext);
        InputSubsystem->AddMappingContext(RuntimeAbilityMappingContext, 50);
    }

    EnhancedInputComponent->BindAction(
        RuntimeDashInputAction,
        ETriggerEvent::Started,
        this,
        &ASkillCharacterBase::Input_Dash);
}

void ASkillCharacterBase::Input_Dash()
{
    ActivateAbilityByTag(DashAbilityTag);
}
