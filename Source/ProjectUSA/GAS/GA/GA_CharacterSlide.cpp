// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterSlide.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "GAS/AT/AT_CheckCharacterCeiling.h"
#include "GAS/AT/AT_CheckCharacterSlope.h"
//#include "GAS/AT/AT_InputCharacterMoveForPeriod.h"
#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_MaintainCharacterVelocity.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "TimerManager.h"
#include "Engine/World.h"


void UGA_CharacterSlide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//
	
	ACharacter* Character = Cast <ACharacter> (ActorInfo->AvatarActor);
	if (Character == nullptr)
	{
		SimpleCancelAbility();
		return;
	}

	SlideForwardDirection = Character->GetPendingMovementInputVector();

	if (HasAuthority(&ActivationInfo))
	{
		if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_Authority
			&& GetAvatarActorFromActorInfo()->GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)

		{
			DoSlide();
		}
	}
	else
	{
		if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			ServerRPC_SetDirectionAndDoSlide(Character->GetPendingMovementInputVector());

			DoSlide();
		}
	}

	//

}

void UGA_CharacterSlide::DoSlide()
{
	ACharacter* Character = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	if (Character == nullptr)
	{
		SimpleCancelAbility();
		return;
	}

	if (SlideForwardDirection.Length() < SMALL_NUMBER)
	{
		SlideForwardDirection = Character->GetActorForwardVector();
	}

	SlideForwardDirection.Normalize();
	
	Character->SetActorRotation(SlideForwardDirection.Rotation());

	bIsSlope = false;
	bIsCeiling = false;
	bIsReleased = false;
	bIsGrounded = true;
	bIsOnceAcitved = false;
	CheckAndRenewEndTimerHandle();

	//UAT_InputCharacterMoveForPeriod* AbilityTaskMove = UAT_InputCharacterMoveForPeriod::GetNewAbilityTask(this, Character, FVector::ForwardVector, -1);
	//AbilityTaskMove->ReadyForActivation();

	UAT_CheckCharacterSlope* AbilityTaskSlope = UAT_CheckCharacterSlope::GetNewAbilityTask_CheckCharacterSlope(this, Character, SlideStartAngle);
	AbilityTaskSlope->OnSlopeFalse.AddDynamic(this, &UGA_CharacterSlide::OnSlopeFalse);
	AbilityTaskSlope->OnSlopeTrue.AddDynamic(this, &UGA_CharacterSlide::OnSlopeTrue);
	AbilityTaskSlope->OnGroundOut.AddDynamic(this, &UGA_CharacterSlide::OnGroundOut);
	AbilityTaskSlope->ReadyForActivation();

	UAT_CheckCharacterCeiling* AbilityTaskCeiling = UAT_CheckCharacterCeiling::GetNewAbilityTask_CheckCharacterCeiling
	(this, Character, SlideDetectCeilingHeight, SlideDetectCeilingRadius);
	AbilityTaskCeiling->OnCeilingFalse.AddDynamic(this, &UGA_CharacterSlide::OnCeilingFalse);
	AbilityTaskCeiling->OnCeilingTrue.AddDynamic(this, &UGA_CharacterSlide::OnCeilingTrue);
	AbilityTaskCeiling->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskPressed = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputPressedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskPressed->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputPressed);
	AbilityTaskPressed->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskReleased = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputReleasedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskReleased->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputReleased);
	AbilityTaskReleased->ReadyForActivation();

	UAT_MaintainCharacterVelocity* AbilityTaskVelocity = UAT_MaintainCharacterVelocity::GetNewAbilityTask_MaintainCharacterVelocity
	(this, TEXT("Slide Veloicty"), SlideStartPower, true);
	AbilityTaskVelocity->ReadyForActivation();

	UAT_ChangeCharacterMovementInfo* AbilityTaskChangeMovementInfo = UAT_ChangeCharacterMovementInfo::GetNewAbilityTask_ChangeCharacterMovementInfo
	(this, Character, SlideMovementInfo);
	OnEndAbility.AddUObject(AbilityTaskChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleEndAbilityTask);
	//OnEndAbility.AddUObject(AbilityTaskChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::ResetCharacterMovementInfo);
	OnCancelAbility.AddUObject(AbilityTaskChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleCancelAbilityTask);
	//OnCancelAbility.AddUObject(AbilityTaskChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::ResetCharacterMovementInfo);
	AbilityTaskChangeMovementInfo->ReadyForActivation();

	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, SlideAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();
}


void UGA_CharacterSlide::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterSlide::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_CharacterSlide::OnPreEndAbilityCallback()
{
	if (!(bIsSlope == true && bIsReleased == false))
	{
		SimpleEndAbility();
	}
}

void UGA_CharacterSlide::OnSlopeTrue()
{
	bIsSlope = true;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(InSlopeEffects);
}

void UGA_CharacterSlide::OnSlopeFalse()
{
	bIsSlope = false;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(OutSlopeEffects);
}

void UGA_CharacterSlide::OnCeilingTrue()
{
	bIsCeiling = true;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(InCeilingEffects);
}

void UGA_CharacterSlide::OnCeilingFalse()
{
	bIsCeiling = false;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(OutCeilingEffects);
}

void UGA_CharacterSlide::OnInputPressed()
{
	bIsReleased = false;
	CheckAndRenewEndTimerHandle();
}

void UGA_CharacterSlide::OnInputReleased()
{
	bIsReleased = true;
	CheckAndRenewEndTimerHandle();
}

void UGA_CharacterSlide::OnGroundOut()
{
	bIsGrounded = false;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(OutGroundEffects);
}


void UGA_CharacterSlide::CheckAndRenewEndTimerHandle()
{
	if (bIsCeiling == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive (EndTimerHandle) == false)
		{
			if (bIsOnceAcitved)
			{
				GetWorld()->GetTimerManager().SetTimer
				(EndTimerHandle, this, &UGA_CharacterSlide::OnPreEndAbilityCallback, SlidePostPeriod, false);
			}
			else
			{
				bIsOnceAcitved = true;
				GetWorld()->GetTimerManager().SetTimer
				(EndTimerHandle, this, &UGA_CharacterSlide::OnPreEndAbilityCallback, SlidePeriod, false);
			}
		}
	}
	
	if (bIsGrounded == false)
	{
		SimpleEndAbility();
	}
}

bool UGA_CharacterSlide::ServerRPC_SetDirectionAndDoSlide_Validate(const FVector& InDirection)
{
	return true;
}

void UGA_CharacterSlide::ServerRPC_SetDirectionAndDoSlide_Implementation(const FVector& InDirection)
{
	SlideForwardDirection = InDirection;

	DoSlide();
}

