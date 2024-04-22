// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"
#include "GAS/AT/AT_LaunchCharacterForPeriod.h"
#include "GAS/AT/AT_WaitDelay.h"
#include "GAS/AT/AT_PlayAnimMontages.h"


//void UGA_CharacterAction::SetAnimBlueprintProperties()
//{
//
//}

void UGA_CharacterAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 컴포넌트 검사
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


	// 방향 설정
	FVector ForwardDirection = MyCharacter->GetActorForwardVector();
	FVector RightDirection = MyCharacter->GetActorRightVector();

	//FVector InputForwardDirection = FVector::ZeroVector;
	//FVector InputRightDirection = FVector::ZeroVector;

	//FVector ForwardDirectionToTarget = FVector::ZeroVector;
	//FVector RightDirectionFromTarget = FVector::ZeroVector;

	switch (DirectionType)
	{
	case ECharacterActionDirectionType::Input:

		// TODO: 만약 멀티를 본격적으로 들어간다면, 아래 부문의 GetPendingMovementInputVector 관련하여 수정할 것!
		if (MyCharacter != nullptr
			&& MyCharacterMovementComponent != nullptr)
		{
			if (MyCharacter->GetPendingMovementInputVector().Length() > SMALL_NUMBER/* != FVector::ZeroVector*/)
			{
				ForwardDirection = MyCharacter->GetPendingMovementInputVector();
				ForwardDirection.Normalize();

				RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
				RightDirection.Normalize();

			}
		}

		MyCharacter->SetActorRotation(ForwardDirection.Rotation());

		break;

	case ECharacterActionDirectionType::Target:

		// TODO 추후 구현

		break;

	case ECharacterActionDirectionType::None:
	default:

		break;
	}

	// 이동 설정
	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		FVector EndLocation = FVector::ZeroVector;
		FVector FinalLaunchVector = FVector::ZeroVector;

		UAT_MoveToLocationByVelocity* AbilityTask_MoveToLocation;
		UAT_LaunchCharacterForPeriod* AbilityTask_LaunchCharacter;

		switch (MoveType)
		{
		case ECharacterActionMoveType::Move:
			EndLocation = MyCharacter->GetActorLocation()
				+ (ForwardDirection * MoveOffsetLocation.X)
				+ (RightDirection * MoveOffsetLocation.Y)
				+ (FVector::UpVector * MoveOffsetLocation.Z);

			AbilityTask_MoveToLocation = UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity
			(this, TEXT("Move"), EndLocation, MoveDuration, MoveCurveFloat, nullptr);

			AbilityTask_MoveToLocation->ReadyForActivation();

			break;

		case ECharacterActionMoveType::Walk:

			// Gameplay Tag로 구현

			break;

		case ECharacterActionMoveType::Launch:
			FinalLaunchVector = (ForwardDirection * MoveLaunchVector.X)
				+ (RightDirection * MoveLaunchVector.Y)
				+ (FVector::UpVector * MoveLaunchVector.Z);

			AbilityTask_LaunchCharacter = UAT_LaunchCharacterForPeriod::GetNewAbilityTask_LaunchCharacterForPeriod
			(this, FinalLaunchVector, bMoveLaunchXYOverride, bMoveLaunchZOverride, MoveLaunchPeriod);

			AbilityTask_LaunchCharacter->ReadyForActivation();

			break;

		case ECharacterActionMoveType::None:
		default:

			break;
		}
	}

	// 애니메이션 설정
	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, ActionAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();


	// 시간 설정
	UAT_WaitDelay* AbilityTaskDelay = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, Period);
	AbilityTaskDelay->OnFinish.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
	AbilityTaskDelay->ReadyForActivation();
}

void UGA_CharacterAction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
