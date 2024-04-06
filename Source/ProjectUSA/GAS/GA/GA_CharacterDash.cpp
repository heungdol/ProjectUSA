// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterDash.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

UGA_CharacterDash::UGA_CharacterDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UGA_CharacterDash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterDash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UGA_CharacterDash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	bool ActivateResult = true;

	ACharacter* MyCharacter = nullptr;
	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(GetAvatarActorFromActorInfo());
	}
	APlayerController* MyPlayerController = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	FVector InputVector = FVector::ZeroVector;

	MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacterMovementComponent != nullptr)
	{
		InputVector = MyCharacter->GetPendingMovementInputVector();
	}

	return ActivateResult;
}

void UGA_CharacterDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		FVector ForwardDirection = MyCharacter->GetActorForwardVector();
		FVector RightDirection = MyCharacter->GetActorRightVector();

		// TODO: 만약 멀티로 개조한다면, 아래 부문의 GetPendingMovementInputVector 관련하여 수정할 것!
		if (MyCharacter->GetPendingMovementInputVector().Length() > SMALL_NUMBER/* != FVector::ZeroVector*/)
		{
			FVector InputVector = MyCharacter->GetPendingMovementInputVector();

			ForwardDirection = InputVector;
			ForwardDirection.Normalize();

			RightDirection = FVector::CrossProduct(FVector::UpVector ,ForwardDirection);
			RightDirection.Normalize();

			MyCharacter->SetActorRotation(ForwardDirection.Rotation());
		}

		FVector EndLocation(0, 0, 0);
		EndLocation = MyCharacter->GetActorLocation() 
			+ (ForwardDirection * DashOffsetLocation.X)
			+ (RightDirection * DashOffsetLocation.Y)
			+ (FVector::UpVector * DashOffsetLocation.Z);

		UAT_MoveToLocationByVelocity* AbilityTask = UAT_MoveToLocationByVelocity::GetNewAbilityTask
		(this, TEXT("Dash"), EndLocation, DashDuration, DashCurveFloat, nullptr);

		AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGA_CharacterDash::OnEndAbilityCallback);
		AbilityTask->ReadyForActivation();

		bIsActivaed = true;
	}
	else
	{
		// Can not activate this ability 
		OnEndAbilityCallback();
	}
}

void UGA_CharacterDash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bIsActivaed == (int8)true)
	{
		bIsActivaed = false;

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
	}
}

void UGA_CharacterDash::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_CharacterDash::OnCancelAbilityCallback()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}




