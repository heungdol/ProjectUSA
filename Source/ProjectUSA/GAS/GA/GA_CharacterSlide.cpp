// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterSlide.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "GAS/AT/AT_CheckCharacterCeiling.h"
#include "GAS/AT/AT_CheckCharacterSlope.h"
#include "GAS/AT/AT_InputCharacterMoveForPeriod.h"
//#include "GAS/AT/AT_WaitDelay.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "TimerManager.h"
#include "Engine/World.h"

//void UGA_CharacterSlide::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
//
//	bIsReleased = false;
//	CheckAndRenewEndTimerHandle();
//
//	UE_LOG(LogTemp, Log, TEXT("Input Pressed"));
//}
//
//void UGA_CharacterSlide::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
//
//	bIsReleased = true;
//	CheckAndRenewEndTimerHandle();
//
//	UE_LOG(LogTemp, Log, TEXT("Input Released"));
//}


//bool UGA_CharacterSlide::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
//{
//	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
//
//	return ActivateResult;
//}

void UGA_CharacterSlide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ACharacter* Character = Cast <ACharacter> (ActorInfo->AvatarActor);
	if (Character == nullptr)
	{
		OnCancelAbilityCallback();
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

	bIsSlope = false;
	bIsCeiling = false;
	bIsReleased = false;
	bIsGrounded = true;
	CheckAndRenewEndTimerHandle();

	UAT_InputCharacterMoveForPeriod* AbilityTaskMove = UAT_InputCharacterMoveForPeriod::GetNewAbilityTask(this, Character, FVector::ForwardVector, -1);
	AbilityTaskMove->ReadyForActivation();

	UAT_CheckCharacterSlope* AbilityTaskSlope = UAT_CheckCharacterSlope::GetNewAbilityTask(this, Character, SlideStartAngle);
	AbilityTaskSlope->OnSlopeFalse.AddUObject(this, &UGA_CharacterSlide::OnSlopeFalse);
	AbilityTaskSlope->OnSlopeTrue.AddUObject(this, &UGA_CharacterSlide::OnSlopeTrue);
	AbilityTaskSlope->OnGroundOut.AddUObject(this, &UGA_CharacterSlide::OnGroundOut);
	AbilityTaskSlope->ReadyForActivation();

	UAT_CheckCharacterCeiling* AbilityTaskCeiling = UAT_CheckCharacterCeiling::GetNewAbilityTask
	(this, Character
		, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f
		, Character->GetCapsuleComponent()->GetScaledCapsuleRadius());
	AbilityTaskCeiling->OnCeilingFalse.AddUObject(this, &UGA_CharacterSlide::OnCeilingFalse);
	AbilityTaskCeiling->OnCeilingTrue.AddUObject(this, &UGA_CharacterSlide::OnCeilingTrue);
	AbilityTaskCeiling->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskPressed = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputPressedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskPressed->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputPressed);
	AbilityTaskPressed->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* AbilityTaskReleased = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, SlideInputReleasedTag, GetAvatarActorFromActorInfo(), false);
	AbilityTaskReleased->Added.AddDynamic(this, &UGA_CharacterSlide::OnInputReleased);
	AbilityTaskReleased->ReadyForActivation();

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


void UGA_CharacterSlide::OnCancelAbilityCallback()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UGA_CharacterSlide::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_CharacterSlide::OnSlopeTrue()
{
	bIsSlope = true;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Slope True"));
}

void UGA_CharacterSlide::OnSlopeFalse()
{
	bIsSlope = false;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Slope False"));
}

void UGA_CharacterSlide::OnCeilingTrue()
{
	bIsCeiling = true;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Ceiling True"));
}

void UGA_CharacterSlide::OnCeilingFalse()
{
	bIsCeiling = false;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Ceiling False"));
}

void UGA_CharacterSlide::OnInputPressed()
{
	bIsReleased = false;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Input Pressed"));
}

void UGA_CharacterSlide::OnInputReleased()
{
	bIsReleased = true;
	CheckAndRenewEndTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("Input Released"));
}

void UGA_CharacterSlide::OnGroundOut()
{
	bIsGrounded = false;
	CheckAndRenewEndTimerHandle();
}


void UGA_CharacterSlide::CheckAndRenewEndTimerHandle()
{
	if (bIsCeiling == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
		//EndTimerHandle.Invalidate();
	}
	else if (bIsSlope == true && bIsReleased == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
		//EndTimerHandle.Invalidate();
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive (EndTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &UGA_CharacterSlide::OnEndAbilityCallback, SlidePeriod, false);
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("%i"), EndTimerHandle.IsValid());
	
	if (bIsGrounded == false)
	{
		OnEndAbilityCallback();
	}
}

