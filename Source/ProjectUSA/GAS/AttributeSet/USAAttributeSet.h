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


	/**
	 *	Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	 *	There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 *	This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	 *
	 *	NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** Called just after any modification happens to an attribute. */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;


	/** Called just after any modification happens to an attribute. */
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// 4_08 무적처리를 위한 함수
	/**
	 *	Called just before modifying the value of an attribute. AttributeSet can make additional modifications here. Return true to continue, or false to throw out the modification.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FUSAAttributeSimpleDynamicDelegate OnOutOfHealth;
	mutable FUSAAttributeSimpleDynamicDelegateFloat OnCurrentHealthChanged;
	mutable FUSAAttributeSimpleDynamicDelegateFloat OnMaxHealthChanged;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;


	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_MaxHealth();



	UPROPERTY(BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentArmor;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseArmor;


	UPROPERTY(BlueprintReadOnly, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	bool bOutOfHealth = false;

	// 체력 관련 어트리뷰트 접근 위함
	friend class AUSACharacterBase;
};
