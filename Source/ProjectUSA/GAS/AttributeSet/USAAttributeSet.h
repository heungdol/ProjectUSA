// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "AbilitySystemComponent.h"

#include "USAAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUSAAttributeSimpleDynamicDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUSAAttributeSimpleDynamicDelegateFloat, float, InFloat);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UUSAAttributeSet();

	ATTRIBUTE_ACCESSORS(UUSAAttributeSet, CurrentHealth);
	ATTRIBUTE_ACCESSORS(UUSAAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UUSAAttributeSet, CurrentArmor);
	ATTRIBUTE_ACCESSORS(UUSAAttributeSet, BaseArmor);
	ATTRIBUTE_ACCESSORS(UUSAAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FUSAAttributeSimpleDynamicDelegate OnOutOfHealth;
	mutable FUSAAttributeSimpleDynamicDelegateFloat OnCurrentHealthChanged;
	mutable FUSAAttributeSimpleDynamicDelegateFloat OnMaxHealthChanged;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentArmor;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseArmor;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	bool bOutOfHealth = false;


protected:

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_MaxHealth();

	
	// 체력 관련 어트리뷰트 접근 위함
	friend class AUSACharacterBase;
};
