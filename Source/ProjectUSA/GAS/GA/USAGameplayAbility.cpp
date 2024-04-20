// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/USAGameplayAbility.h"

#include "AbilitySystemComponent.h"

#include "ProjectUSA.h"

void UUSAGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);

}

void UUSAGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyEffectsViaArray(ActivateAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnActivateAbility.Broadcast();
	
	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Activate Ability"));
}

void UUSAGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	ApplyEffectsViaArray(CancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnCancelAbility.Broadcast();
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ApplyEffectsViaArray(PostCancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Cancel Ability"));
}

void UUSAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ApplyEffectsViaArray(EndAbilityEffects, Handle, ActorInfo, ActivationInfo);
	
	OnEndAbility.Broadcast();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ApplyEffectsViaArray(PostEndAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("End Ability"));
}

void UUSAGameplayAbility::SimpleCancelAbility()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UUSAGameplayAbility::SimpleEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UUSAGameplayAbility::ApplyEffectsViaArray
(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects
	, const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, float GameplayEffectLevel, int32 Stacks)
{
	for (const auto& GameplayEffectClass : GameplayEffects)
	{
		if (GameplayEffectClass == nullptr)
		{
			continue;
		}

		const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, GameplayEffect, GameplayEffectLevel, Stacks);
	}
}

void UUSAGameplayAbility::ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects)
{
	ApplyEffectsViaArray(GameplayEffects, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}
