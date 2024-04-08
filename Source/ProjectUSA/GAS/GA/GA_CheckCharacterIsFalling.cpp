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

	UAT_CheckCharacterIsFalling* AbilityTask = UAT_CheckCharacterIsFalling::GetNewAbilityTask(this, MyCharacterMovementComponent);
	
	AbilityTask->OnPositiveFalling.AddUObject(this, &UGA_CheckCharacterIsFalling::OnPositiveFallingCallback);
	AbilityTask->OnNegativeFalling.AddUObject(this, &UGA_CheckCharacterIsFalling::OnNegativeFallingCallback);
	AbilityTask->OnGrounded.AddUObject(this, &UGA_CheckCharacterIsFalling::OnGroundedCallback);
	AbilityTask->OnFinished.AddUObject(this, &UGA_CheckCharacterIsFalling::OnFinishedCallback);

	AbilityTask->ReadyForActivation();
}

void UGA_CheckCharacterIsFalling::OnPositiveFallingCallback()
{
	ApplyEffectsByArray(PositiveFallingEffects);

	//for (const auto& PositiveFallingEffect : PositiveFallingEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(PositiveFallingEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UGA_CheckCharacterIsFalling::OnNegativeFallingCallback()
{
	ApplyEffectsByArray(NegativeFallingEffects);

	//for (const auto& NegativeFallingEffect : NegativeFallingEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(NegativeFallingEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UGA_CheckCharacterIsFalling::OnGroundedCallback()
{
	ApplyEffectsByArray(GroundedEffects);

	//for (const auto& GroundedEffect : GroundedEffects)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GroundedEffect);
	//	if (GameplayEffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffectSpecHandle);
	//	}
	//}
}

void UGA_CheckCharacterIsFalling::OnFinishedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
