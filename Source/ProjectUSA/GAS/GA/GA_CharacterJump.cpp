// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterJump.h"

#include "GAS/AT/AT_LaunchCharacterForPeriod.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "ProjectUSA.h"


UGA_CharacterJump::UGA_CharacterJump()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_CharacterJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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

	FVector JumpVelocity(0, 0, (CharacterMovementComponent->JumpZVelocity));
	float JumpMaxHoldTime = Character->GetJumpMaxHoldTime();

	if (bIsUsingCharacterJumpPower == false)
	{
		JumpVelocity = FVector::UpVector * CustomJumpPower;
		JumpMaxHoldTime = CustomJumpMaxHoldTime;
	}

	UAT_LaunchCharacterForPeriod* AbilityTask = UAT_LaunchCharacterForPeriod::GetNewAbilityTask_LaunchCharacterForPeriod
	(this, JumpVelocity, false, true, JumpMaxHoldTime);
	AbilityTask->OnFinished.AddDynamic(this, &UGA_CharacterJump::SimpleEndAbility);

	AbilityTask->ReadyForActivation();

	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, JumpAnimMontageData);
	//OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	//OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);

	AbilityTaskMontage->ReadyForActivation();
}

void UGA_CharacterJump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//USA_LOG(LogTemp, Log, TEXT("Jump Released..."));

	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

bool UGA_CharacterJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return bResult;
}

void UGA_CharacterJump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


//void UGA_CharacterJump::OnCancelAbilityCallback()
//{
//	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
//}
//
//void UGA_CharacterJump::OnEndAbilityCallback()
//{
//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
//}

