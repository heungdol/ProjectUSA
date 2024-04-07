// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterStomp.h"

#include "GAS/AT/AT_MoveToGround.h"
#include "GAS/AT/AT_WaitDelay.h"
//#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "AbilitySystemBlueprintLibrary.h"


UGA_CharacterStomp::UGA_CharacterStomp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsActivaed = false;
}

void UGA_CharacterStomp::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterStomp::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}


bool UGA_CharacterStomp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return ActivateResult;
}

void UGA_CharacterStomp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		UAT_WaitDelay* AbilityTask0 = UAT_WaitDelay::GetNewAbilityTask(this, StompPreDelay);
		UAT_MoveToGround* AbilityTask1 = UAT_MoveToGround::GetNewAbilityTask(this, TEXT("Stomp"), StompMoveSpeed);
		UAT_WaitDelay* AbilityTask2 = UAT_WaitDelay::GetNewAbilityTask(this, StompPostDelay);

		OnPreEffectsCallback();

		AbilityTask0->OnFinish.AddDynamic(AbilityTask1, &UAT_MoveToGround::ReadyForActivation);
		AbilityTask0->OnFinish.AddDynamic(this, &UGA_CharacterStomp::OnActiveEffectsCallback);

		AbilityTask1->OnGroundReached.AddUObject(AbilityTask2, &UAT_WaitDelay::ReadyForActivation);
		AbilityTask1->OnGroundReached.AddUObject(this, &UGA_CharacterStomp::OnEndEffectsCallback);

		AbilityTask2->OnFinish.AddDynamic(this, &UGA_CharacterStomp::OnEndAbilityCallback);
		AbilityTask2->OnFinish.AddDynamic(this, &UGA_CharacterStomp::OnPostEffectsCallback);

		AbilityTask0->ReadyForActivation();

		bIsActivaed = true;
	}
	else
	{
		OnEndAbilityCallback();
	}
}


void UGA_CharacterStomp::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterStomp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_CharacterStomp::OnCancelAbilityCallback()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UGA_CharacterStomp::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UGA_CharacterStomp::OnPreEffectsCallback()
{
	ApplyEffectsByArray(PreGameplayEffects);
}

void UGA_CharacterStomp::OnActiveEffectsCallback()
{
	ApplyEffectsByArray(ActiveGameplayEffects);
}

void UGA_CharacterStomp::OnEndEffectsCallback()
{
	ApplyEffectsByArray(EndGameplayEffects);
}

void UGA_CharacterStomp::OnPostEffectsCallback()
{
	ApplyEffectsByArray(PostGameplayEffects);
}
