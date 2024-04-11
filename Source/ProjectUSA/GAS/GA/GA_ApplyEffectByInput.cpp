// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ApplyEffectByInput.h"

#include "GameplayEffect.h"

UGA_ApplyEffectByInput::UGA_ApplyEffectByInput()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ApplyEffectByInput::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (bIsInputPressedOnce == false)
	{
		//FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(InputPressedEffect);
		//if (GameplayEffectSpecHandle.IsValid())
		//{
		//	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, GameplayEffectSpecHandle);
		//}

		ApplyEffectsByArray(InputPressedEffects);

		bIsInputPressedOnce = true;
	}
}

void UGA_ApplyEffectByInput::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bIsInputPressedOnce == true)
	{
		//FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(InputReleasedEffect);
		//if (GameplayEffectSpecHandle.IsValid())
		//{
		//	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, GameplayEffectSpecHandle);
		//}

		ApplyEffectsByArray(InputReleasedEffects);

		bIsInputPressedOnce = false;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_ApplyEffectByInput::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UGA_ApplyEffectByInput::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}

void UGA_ApplyEffectByInput::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);


}




