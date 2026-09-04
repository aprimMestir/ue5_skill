#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SKILL_API USkillAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    USkillAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Mana)
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, Mana)

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxMana)
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, MaxMana)

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxStamina)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(USkillAttributeSet, MaxStamina)

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
};
