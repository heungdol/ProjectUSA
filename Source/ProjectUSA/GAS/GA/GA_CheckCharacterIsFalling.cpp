// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CheckCharacterIsFalling.h"
#include "GAS/AT/AT_CheckCharacterIsFalling.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UGA_CheckCharacterIsFalling::UGA_CheckCharacterIsFalling()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_CheckCharacterIsFalling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (MyCharacter == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	UCharacterMovementComponent* MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	if (MyCharacterMovementComponent == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	UAT_CheckCharacterIsFalling* AbilityTask = UAT_CheckCharacterIsFalling::SpawnActor(this, MyCharacterMovementComponent);
	
	AbilityTask->OnPositiveFalling.AddUObject(this, &UGA_CheckCharacterIsFalling::CallbackPositiveFalling);
	AbilityTask->OnNegativeFalling.AddUObject(this, &UGA_CheckCharacterIsFalling::CallbackNegativeFalling);
	AbilityTask->OnGrounded.AddUObject(this, &UGA_CheckCharacterIsFalling::CallbackGrounded);
	AbilityTask->OnFinished.AddUObject(this, &UGA_CheckCharacterIsFalling::CallbackFinished);

	AbilityTask->ReadyForActivation();
}

void UGA_CheckCharacterIsFalling::CallbackPositiveFalling()
{
	for (const auto& PositiveFallingEffect : PositiveFallingEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(PositiveFallingEffect);
		if (GameplayEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
		}
	}
}

void UGA_CheckCharacterIsFalling::CallbackNegativeFalling()
{
	for (const auto& NegativeFallingEffect : NegativeFallingEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(NegativeFallingEffect);
		if (GameplayEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
		}
	}
}

void UGA_CheckCharacterIsFalling::CallbackGrounded()
{
	for (const auto& GroundedEffect : GroundedEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GroundedEffect);
		if (GameplayEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
		}
	}
}

void UGA_CheckCharacterIsFalling::CallbackFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
