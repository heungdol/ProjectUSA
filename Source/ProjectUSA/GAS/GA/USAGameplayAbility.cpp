// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/USAGameplayAbility.h"

#include "AbilitySystemComponent.h"

void UUSAGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyEffectsViaArray(ActivateAbilityEffects);
	
	OnActivateAbility.Broadcast();
}

void UUSAGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	// CancelAbility 함수 전후로 알맞은 이펙트 호출할 것

	ApplyEffectsViaArray(CancelAbilityEffects);

	OnCancelAbility.Broadcast();
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ApplyEffectsViaArray(PostCancelAbilityEffects);
}

void UUSAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// EndAbility 함수 전후로 알맞은 이펙트 호출할 것

	ApplyEffectsViaArray(EndAbilityEffects);
	
	OnEndAbility.Broadcast();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ApplyEffectsViaArray(PostEndAbilityEffects);
}

void UUSAGameplayAbility::SimpleCancelAbility()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UUSAGameplayAbility::SimpleEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UUSAGameplayAbility::ApplyEffectsViaArray(const TArray<TSubclassOf<UGameplayEffect>>& GameplayEffects/*, float GameplayEffectLevel = 0.0f, int32 Stacks = 1*/)
{
	for (const auto& GameplayEffectClass : GameplayEffects)
	{
		if (GameplayEffectClass == nullptr)
		{
			continue;
		}

		BP_ApplyGameplayEffectToOwner(GameplayEffectClass);
	
		//if (GameplayEffectClass)
		//{
		//	const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		//	ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffect, GameplayEffectLevel, Stacks);
		//}
	}
}
