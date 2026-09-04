#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SkillCharacterBase.generated.h"

class USkillAbilitySystemComponent;
class USkillAttributeSet;
class USkillGameplayAbility;

UCLASS()
class SKILL_API ASkillCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ASkillCharacterBase();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UFUNCTION(BlueprintCallable, Category="Abilities")
    void GrantStartupAbilities();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
    TObjectPtr<USkillAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
    TObjectPtr<USkillAttributeSet> AttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
    TArray<TSubclassOf<USkillGameplayAbility>> StartupAbilities;

protected:
    virtual void BeginPlay() override;

private:
    void InitializeAbilityActorInfo();
    bool bStartupAbilitiesGranted = false;
};
