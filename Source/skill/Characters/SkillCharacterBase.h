#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "SkillCharacterBase.generated.h"

class USkillAbilitySystemComponent;
class USkillAttributeSet;
class USkillGameplayAbility;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SKILL_API ASkillCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ASkillCharacterBase();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Abilities")
    void GrantStartupAbilities();

    UFUNCTION(BlueprintCallable, Category="Abilities")
    bool ActivateAbilityByTag(FGameplayTag AbilityTag);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
    TObjectPtr<USkillAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
    TObjectPtr<USkillAttributeSet> AttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
    TArray<TSubclassOf<USkillGameplayAbility>> StartupAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities|Input")
    bool bEnableRuntimeDashInput = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities|Input")
    FKey DashKey;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities|Input")
    FGameplayTag DashAbilityTag;

protected:
    virtual void BeginPlay() override;

private:
    void InitializeAbilityActorInfo();
    void ConfigureRuntimeAbilityInput(UInputComponent* PlayerInputComponent);
    void Input_Dash();

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> RuntimeDashInputAction;

    UPROPERTY(Transient)
    TObjectPtr<UInputMappingContext> RuntimeAbilityMappingContext;

    bool bStartupAbilitiesGranted = false;
};
