// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterDash.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "ProjectUSA.h"

UGA_CharacterDash::UGA_CharacterDash()
{

}

void UGA_CharacterDash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterDash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateAbilityUsingTargetVector(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_CharacterDash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// ==============================================================================================================

void UGA_CharacterDash::CalculateTargetVector()
{
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		FVector DashForwardDirection = MyCharacter->GetPendingMovementInputVector();

		if (DashForwardDirection.Length() > SMALL_NUMBER)
		{
			TargetVector_Move = DashForwardDirection;
		}
		else
		{
			TargetVector_Move = MyCharacter->GetActorForwardVector();
		}
	}
}

void UGA_CharacterDash::DoSomethingWithTargetVector()
{
	if (GetIsAbleToActivateCondition() == false)
	{
		SimpleCancelAbility();
		return;
	}

	ACharacter* MyCharacter = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	FVector	ForwardDirection = TargetVector_Move;
	ForwardDirection.Normalize();

	FVector	RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
	RightDirection.Normalize();

	MyCharacter->SetActorRotation(ForwardDirection.Rotation());

	FVector EndLocation(0, 0, 0);
	EndLocation = MyCharacter->GetActorLocation()
		+ (ForwardDirection * DashOffsetLocation.X)
		+ (RightDirection * DashOffsetLocation.Y)
		+ (FVector::UpVector * DashOffsetLocation.Z);

	UAT_MoveToLocationByVelocity* AbilityTask = UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity
	(this, TEXT("Dash"), EndLocation, MoveAfterVelocity, DashDuration, DashCurveFloat, nullptr);

	AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGA_CharacterDash::SimpleEndAbility);
	AbilityTask->ReadyForActivation();

	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, DashAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);

	AbilityTaskMontage->ReadyForActivation();
}

bool UGA_CharacterDash::GetIsAbleToActivateCondition()
{
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter == nullptr
		|| MyCharacterMovementComponent == nullptr)
	{
		return false;
	}

	return true;
}

// ==============================================================================================================

//bool UGA_CharacterDash::ServerRPC_SetDashForwardDirection_Validate(const FVector& InDirection)
//{
//	return true;
//}
//
//void UGA_CharacterDash::ServerRPC_SetDashForwardDirection_Implementation(const FVector& InDirection)
//{
//	USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("called in server"));
//
//	DashForwardDirection = InDirection;
//
//	DoDash();
//}
