// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSet/USAAttributeSet.h"

#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

#include "ProjectUSA.h"


UUSAAttributeSet::UUSAAttributeSet() :
	CurrentHealth(100.0f),
	MaxHealth(100.0f),
	CurrentArmor(0.0f),
	BaseArmor(0.0f),
	Damage (0.0f)
{
	InitCurrentHealth(GetMaxHealth());

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

	if (FMath::Abs(NewValue - OldValue) < SMALL_NUMBER)
	{
		return;
	}

	if (Attribute == GetDamageAttribute() && NewValue > SMALL_NUMBER)
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - NewValue, MinHealth, GetMaxHealth()));
		SetDamage(0.0f);

		if (GetCurrentHealth() <= SMALL_NUMBER
			&& bOutOfHealth == false)
		{
			UE_LOG(LogTemp, Log, TEXT("Death condition from attributeset"));

			bOutOfHealth = true;

			OnOutOfHealth.Broadcast();
		}
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

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), MinHealth, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);

		if (GetCurrentHealth() <= SMALL_NUMBER
			&& bOutOfHealth == false)
		{
			UE_LOG(LogTemp, Log, TEXT("Death condition from attributeset"));

			bOutOfHealth = true;

			OnOutOfHealth.Broadcast();
		}
	}
}

void UUSAAttributeSet::OnRep_CurrentHealth()
{
	UE_LOG(LogTemp, Log, TEXT("Replicated Health"));

	OnCurrentHealthChanged.Broadcast(GetCurrentHealth());
}

void UUSAAttributeSet::OnRep_MaxHealth()
{
	OnMaxHealthChanged.Broadcast(GetMaxHealth());
}


void UUSAAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUSAAttributeSet, CurrentHealth);
	DOREPLIFETIME(UUSAAttributeSet, MaxHealth);
	DOREPLIFETIME(UUSAAttributeSet, CurrentArmor);
	DOREPLIFETIME(UUSAAttributeSet, BaseArmor);
	DOREPLIFETIME(UUSAAttributeSet, Damage);
}
