// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ApplyEffectByInput.h"

#include "GameplayEffect.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"

#include "ProjectUSA.h"


UGA_ApplyEffectByInput::UGA_ApplyEffectByInput()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ApplyEffectByInput::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ApplyEffectByInput::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (bIsInputPressedOnce == false)
	{
		bIsInputPressedOnce = true;
	}
}

void UGA_ApplyEffectByInput::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bIsInputPressedOnce == true)
	{
		bIsInputPressedOnce = false;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
