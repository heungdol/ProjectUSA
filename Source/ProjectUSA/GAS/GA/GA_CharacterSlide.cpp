// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterSlide.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "GAS/AT/AT_CheckCharacterCeiling.h"
#include "GAS/AT/AT_CheckCharacterSlope.h"
#include "GAS/AT/AT_InputCharacterMoveForPeriod.h"
#include "GAS/AT/AT_PlayAnimMontages.h"

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
	FVector VelocityDirection = Character->GetCharacterMovement()->Velocity;

	if (Character->GetPendingMovementInputVector().Length() > SMALL_NUMBER)
	{
		VelocityDirection = Character->GetPendingMovementInputVector();
	}

	VelocityDirection.Normalize();

	Character->SetActorRotation(VelocityDirection.Rotation());

	Character->GetCharacterMovement()->Velocity = VelocityDirection * SlideStartPower;
	Character->GetCharacterMovement()->UpdateComponentVelocity();

	//

	bIsSlope = false;
	bIsCeiling = false;
	bIsReleased = false;
	bIsGrounded = true;
	bIsOnceAcitved = false;
	CheckAndRenewEndTimerHandle();

	UAT_InputCharacterMoveForPeriod* AbilityTaskMove = UAT_InputCharacterMoveForPeriod::GetNewAbilityTask(this, Character, FVector::ForwardVector, -1);
	AbilityTaskMove->ReadyForActivation();

	UAT_CheckCharacterSlope* AbilityTaskSlope = UAT_CheckCharacterSlope::GetNewAbilityTask(this, Character, SlideStartAngle);
	AbilityTaskSlope->OnSlopeFalse.AddUObject(this, &UGA_CharacterSlide::OnSlopeFalse);
	AbilityTaskSlope->OnSlopeTrue.AddUObject(this, &UGA_CharacterSlide::OnSlopeTrue);
	AbilityTaskSlope->OnGroundOut.AddUObject(this, &UGA_CharacterSlide::OnGroundOut);
	AbilityTaskSlope->ReadyForActivation();

	UAT_CheckCharacterCeiling* AbilityTaskCeiling = UAT_CheckCharacterCeiling::GetNewAbilityTask
	(this, Character, SlideDetectCeilingHeight, SlideDetectCeilingRadius);
	AbilityTaskCeiling->OnCeilingFalse.AddUObject(this, &UGA_CharacterSlide::OnCeilingFalse);
	AbilityTaskCeiling->OnCeilingTrue.AddUObject(this, &UGA_CharacterSlide::OnCeilingTrue);
	AbilityTaskCeiling->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskPressed = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputPressedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskPressed->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputPressed);
	AbilityTaskPressed->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskReleased = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputReleasedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskReleased->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputReleased);
	AbilityTaskReleased->ReadyForActivation();

	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask(this, SlideAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::OnEndTaskCallback);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::OnEndTaskCallback);
	AbilityTaskMontage->ReadyForActivation();

	//UAT_WaitDelay* AbilityTaskWait = UAT_WaitDelay::GetNewAbilityTask(this, SlidePeriod);
	//AbilityTaskWait->OnFinish.AddDynamic(this, &UGA_CharacterSlide::OnEndAbilityCallback);
	//AbilityTaskWait->ReadyForActivation();
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

	//UE_LOG(LogTemp, Log, TEXT("Slope True"));
}

void UGA_CharacterSlide::OnSlopeFalse()
{
	bIsSlope = false;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(OutSlopeEffects);

	//UE_LOG(LogTemp, Log, TEXT("Slope False"));
}

void UGA_CharacterSlide::OnCeilingTrue()
{
	bIsCeiling = true;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(InCeilingEffects);

	//UE_LOG(LogTemp, Log, TEXT("Ceiling True"));
}

void UGA_CharacterSlide::OnCeilingFalse()
{
	bIsCeiling = false;
	CheckAndRenewEndTimerHandle();

	ApplyEffectsViaArray(OutCeilingEffects);

	//UE_LOG(LogTemp, Log, TEXT("Ceiling False"));
}

void UGA_CharacterSlide::OnInputPressed()
{
	bIsReleased = false;
	CheckAndRenewEndTimerHandle();

	//UE_LOG(LogTemp, Log, TEXT("Input Pressed"));
}

void UGA_CharacterSlide::OnInputReleased()
{
	bIsReleased = true;
	CheckAndRenewEndTimerHandle();

	//UE_LOG(LogTemp, Log, TEXT("Input Released"));
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
		//EndTimerHandle.Invalidate();
	}
	//else if (bIsSlope == true && bIsReleased == false)
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	//	//EndTimerHandle.Invalidate();
	//}
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

	//UE_LOG(LogTemp, Log, TEXT("%i"), EndTimerHandle.IsValid());
	
	if (bIsGrounded == false)
	{
		SimpleEndAbility();
	}
}

