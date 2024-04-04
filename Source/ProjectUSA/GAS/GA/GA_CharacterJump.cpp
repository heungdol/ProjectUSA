// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterJump.h"

#include "GAS/AT/AT_SetCharacterVelocityForPeriod.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_CharacterJump::UGA_CharacterJump()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_CharacterJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	//{
	//	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	//	{
	//		return;
	//	}

	//	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	//	if (Character == nullptr)
	//	{
	//		return;
	//	}

	//	Character->Jump();
	//}

	//bIsJumping = true;

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character == nullptr)
	{
		return;
	}

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if (CharacterMovementComponent == nullptr)
	{
		return;
	}

	FVector JumpVelocity(0, 0, CharacterMovementComponent->JumpZVelocity);

	UAT_SetCharacterVelocityForPeriod* AbilityTask = UAT_SetCharacterVelocityForPeriod::GetNewAbilityTask
	(this, JumpVelocity, false, true, Character->GetJumpMaxHoldTime());

	AbilityTask->OnFinished.AddUObject(this, &UGA_CharacterJump::OnEndCallback);

	AbilityTask->ReadyForActivation();
}

void UGA_CharacterJump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	//{
	//}
	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

bool UGA_CharacterJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return bResult;

	//if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	//{
	//	return false;
	//}

	//const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	//if (Character == nullptr)
	//{
	//	return false;
	//}

	//return (Character->CanJump());
}

void UGA_CharacterJump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	//if (Character == nullptr)
	//{
	//	return;
	//}

	//Character->StopJumping();

	//bIsJumping = false;
}

void UGA_CharacterJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//bIsJumping = false;
}

void UGA_CharacterJump::OnEndCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

