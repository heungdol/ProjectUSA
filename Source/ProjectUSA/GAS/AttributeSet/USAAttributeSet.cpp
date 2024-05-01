// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSet/USAAttributeSet.h"

#include "GameplayEffectExtension.h"


UUSAAttributeSet::UUSAAttributeSet() :
	Health (100.0f),
	MaxHealth (100.0f),
	Damage (0.0f)
{
	InitHealth(GetMaxHealth());

	bOutOfHealth = false;
}

void UUSAAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetDamageAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UUSAAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	const float MinHealth = 0.0f;

	//if (Attribute == GetHealthAttribute())
	//{
	//	SetHealth(FMath::Clamp(NewValue, MinHealth, GetMaxHealth()));
	//}

	if (Attribute == GetDamageAttribute() && NewValue > SMALL_NUMBER)
	{
		SetHealth(FMath::Clamp(GetHealth() - NewValue, MinHealth, GetMaxHealth()));
		SetDamage(0.0f);

		//if (NewValue > 0.0f)
		//{
		//}
	}
}

bool UUSAAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	bool bOriginalResult = Super::PreGameplayEffectExecute(Data);

	// ... 무적 및 패링 관련 추가

	return bOriginalResult;
}

void UUSAAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const float MinHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}


	// ...


	if (GetHealth() <= 0.0f && bOutOfHealth == false)
	{
		bOutOfHealth = true;

		OnOutOfHealth.Broadcast();
	}
}




