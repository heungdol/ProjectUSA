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
		SimpleCancelAbility();
	}

	UCharacterMovementComponent* MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	if (MyCharacterMovementComponent == nullptr)
	{
		SimpleCancelAbility();
	}

	UAT_CheckCharacterIsFalling* AbilityTask = UAT_CheckCharacterIsFalling::GetNewAbilityTask_CheckCharacterIsFalling(this, MyCharacterMovementComponent);
	
	AbilityTask->OnPositiveFalling.AddDynamic(this, &UGA_CheckCharacterIsFalling::OnPositiveFallingCallback);
	AbilityTask->OnNegativeFalling.AddDynamic(this, &UGA_CheckCharacterIsFalling::OnNegativeFallingCallback);
	AbilityTask->OnGrounded.AddDynamic(this, &UGA_CheckCharacterIsFalling::OnGroundedCallback);
	AbilityTask->OnFinished.AddDynamic(this, &UGA_CheckCharacterIsFalling::OnFinishedCallback);

	AbilityTask->ReadyForActivation();
}

void UGA_CheckCharacterIsFalling::OnPositiveFallingCallback()
{
	ApplyEffectsViaArray(PositiveFallingEffects);
}

void UGA_CheckCharacterIsFalling::OnNegativeFallingCallback()
{
	ApplyEffectsViaArray(NegativeFallingEffects);
}

void UGA_CheckCharacterIsFalling::OnGroundedCallback()
{
	ApplyEffectsViaArray(GroundedEffects);
}

void UGA_CheckCharacterIsFalling::OnFinishedCallback()
{
	SimpleEndAbility();
}
