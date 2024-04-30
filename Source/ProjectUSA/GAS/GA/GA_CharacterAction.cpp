// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"
#include "GAS/AT/AT_LaunchCharacterForPeriod.h"
#include "GAS/AT/AT_WaitDelay.h"
#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "ProjectUSA.h"


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
	ForwardDirection = MyCharacter->GetActorForwardVector();
	RightDirection = MyCharacter->GetActorRightVector();

	switch (DirectionType)
	{

	// 일단 인풋 특별 대우
	case ECharacterActionDirectionType::Input:

		if (MyCharacter != nullptr
			&& MyCharacterMovementComponent != nullptr)
		{
			SetForwardAndRightDirection(MyCharacter->GetPendingMovementInputVector());

			if (HasAuthority(&ActivationInfo))
			{
				if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_Authority
					&& GetAvatarActorFromActorInfo()->GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
				{
					DoAction();
				}
			}
			else
			{
				if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
				{
					ServerRPC_SetActionDirecitonAndDoAction(MyCharacter->GetPendingMovementInputVector());

					DoAction();
				}
			}
		}

		break;

	case ECharacterActionDirectionType::Target:

		// TODO 추후 타깃 바라보는 과정 구현
		// 월드에 존재하는 하나의 액터를 바라보는 것이기 때문에, input 처럼 RPC를 이용할 필요는 없을 것 같다.

		DoAction();

		break;

	case ECharacterActionDirectionType::None:
	default:

		DoAction();

		break;
	}

	DoSomethingInBlueprint_Activate();
}

void UGA_CharacterAction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	DoSomethingInBlueprint_Cancel();
}

void UGA_CharacterAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	DoSomethingInBlueprint_End();
}


//

//bool UGA_CharacterAction::ServerRPC_SetActionDirecitonAndDoAction_Validate(const FVector& InDirection)
//{
//	return true;
//}

void UGA_CharacterAction::ServerRPC_SetActionDirecitonAndDoAction_Implementation(const FVector& InDirection)
{
	SetForwardAndRightDirection(InDirection);

	DoAction();
}

//

void UGA_CharacterAction::SetForwardAndRightDirection(const FVector& InDirection)
{
	if (InDirection.SquaredLength() > SMALL_NUMBER)
	{
		ForwardDirection = InDirection;
		ForwardDirection.Normalize();

		RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
		RightDirection.Normalize();
	}
}

void UGA_CharacterAction::ServerRPC_PlayAnimMontageTask_Implementation()
{
	MulticastRPC_PlayAnimMontageTask();
}

void UGA_CharacterAction::MulticastRPC_PlayAnimMontageTask_Implementation()
{
	// 애니메이션 설정
	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, ActionAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();

	//UE_LOG (LogTemp, Log, TEXT("Setted Animation Montage"));
}

//

void UGA_CharacterAction::DoAction()
{
	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("B"));

	// 컴포넌트 검사
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

	// 이동 설정
	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		MyCharacter->SetActorRotation(ForwardDirection.Rotation());

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

	// Action 종료 조건 설정
	UAbilityTask_WaitGameplayTagAdded* WaitGameplayTagAdded = nullptr;
	UAbilityTask_WaitGameplayTagRemoved* WaitGameplayTagRemoved = nullptr;
	UAT_WaitDelay* AbilityTaskDelay = nullptr;

	switch (EndType)
	{

	case ECharacterActionEndType::WaitTagAdded:

		WaitGameplayTagAdded = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, EndGameplayTag);
		WaitGameplayTagAdded->Added.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		WaitGameplayTagAdded->ReadyForActivation();

		break;

	case ECharacterActionEndType::WaitTagRemoved:

		WaitGameplayTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, EndGameplayTag);
		WaitGameplayTagRemoved->Removed.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		WaitGameplayTagRemoved->ReadyForActivation();

		break;

	case ECharacterActionEndType::WaitTime:
	default:

		AbilityTaskDelay = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, Period);
		AbilityTaskDelay->OnFinish.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		AbilityTaskDelay->ReadyForActivation();

		break;

	}

	// 스폰 설정
	UAT_SpawnActors* AbiltiyTaskSpawn = UAT_SpawnActors::GetNewAbilityTask_SpawnActors(this, SpawnActorData);
	AbiltiyTaskSpawn->ReadyForActivation();

	// 애니메이션 설정
	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, ActionAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();

	
	//if (HasAuthority(&CurrentActivationInfo))
	//{		 
	
	// 공격 설정
	UAT_TraceAttack* AbiltiyTaskAttack = UAT_TraceAttack::GetNewAbilityTask_TraceAttack(this, AttackTraceData);
	AbiltiyTaskAttack->ReadyForActivation();
	
	//	USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("C"));
	//}

}
