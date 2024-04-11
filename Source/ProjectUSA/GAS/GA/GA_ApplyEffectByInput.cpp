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
		ApplyEffectsViaArray(InputPressedEffects);

		bIsInputPressedOnce = true;
	}
}

void UGA_ApplyEffectByInput::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bIsInputPressedOnce == true)
	{
		ApplyEffectsViaArray(InputReleasedEffects);

		bIsInputPressedOnce = false;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
