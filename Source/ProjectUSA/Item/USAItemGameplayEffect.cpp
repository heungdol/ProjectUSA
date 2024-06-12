// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USAItemGameplayEffect.h"

#include "AbilitySystemComponent.h"

void AUSAItemGameplayEffect::ActiveUSAItem(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	Super::ActiveUSAItem(InASC, InCharacter);

	if (InASC && ItemGameplayEffect)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = InASC->MakeOutgoingSpec(ItemGameplayEffect, 0.0f, FGameplayEffectContextHandle());
		InASC->BP_ApplyGameplayEffectSpecToSelf(GameplayEffectSpecHandle);
	}
}
