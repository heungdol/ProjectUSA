// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/USAGameplayAbility.h"

void UUSAGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyEffectsByArray(ActivateAbilityEffects);

	//for (const auto& ActivateAbilityEffect : ActivateAbilityEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ActivateAbilityEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UUSAGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ApplyEffectsByArray(CancelAbilityEffects);

	//for (const auto& CancelAbilityEffect : CancelAbilityEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(CancelAbilityEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UUSAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ApplyEffectsByArray(EndAbilityEffects);

	//for (const auto& EndAbilityEffect : EndAbilityEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(EndAbilityEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UUSAGameplayAbility::ApplyEffectsByArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects)
{
	for (const auto& GameplayEffect : GameplayEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GameplayEffect);
		if (GameplayEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
		}
	}
}
