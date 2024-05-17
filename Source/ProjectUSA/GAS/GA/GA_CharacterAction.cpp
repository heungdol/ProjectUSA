// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"
#include "GAS/AT/AT_LaunchCharacterForPeriod.h"
#include "GAS/AT/AT_WaitDelay.h"
#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_WaitGameplayTag.h"

//#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "Character/USACharacterBase.h"

#include "Kismet/KismetSystemLibrary.h"

#include "ProjectUSA.h"


void UGA_CharacterAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//CommitAbility

	if (GetIsAbleToActivateCondition() == false)
	{
		SimpleEndAbility();
		return;
	}

	ActivateAbilityUsingTargetVector(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	K2_DoSomething_Activate();
}

void UGA_CharacterAction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	K2_DoSomething_Cancel();
}

void UGA_CharacterAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	K2_DoSomething_End();
}

void UGA_CharacterAction::CalculateTargetVector()
{
	ACharacter* MyCharacter = nullptr;
	AUSACharacterBase* MyUSACharacter = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyUSACharacter = Cast <AUSACharacterBase>(MyCharacter);

		TargetVector = MyCharacter->GetActorForwardVector();
	}

	switch (DirectionType)
	{
		case ECharacterActionDirectionType::Input:

		if (MyUSACharacter != nullptr
			&& MyUSACharacter->GetUSACharacterDirection_InputMovement().Length() > SMALL_NUMBER)
		{
			TargetVector = MyUSACharacter->GetUSACharacterDirection_InputMovement();
		}
		else if (MyCharacter != nullptr
			&& MyCharacter->GetPendingMovementInputVector().Length() > SMALL_NUMBER)
		{
			TargetVector = MyCharacter->GetPendingMovementInputVector();
		}

		break;

		case ECharacterActionDirectionType::Target:
		if (MyUSACharacter != nullptr
			&& MyUSACharacter->GetUSACharacterDirection_Target().Length() > SMALL_NUMBER)
		{
			TargetVector = MyUSACharacter->GetUSACharacterDirection_Target();
		}

		break;

		case ECharacterActionDirectionType::None:
		default:


		break;
	}
}

void UGA_CharacterAction::DoSomethingWithTargetVector()
{
	if (GetIsAbleToActivateCondition() == false)
	{
		SimpleCancelAbility();
		return;
	}

	// 컴포넌트 검사
	// 이동 설정
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
		FVector	ForwardDirection = TargetVector;
		ForwardDirection.Normalize();

		FVector	RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
		RightDirection.Normalize();

		FVector EndLocation = FVector::ZeroVector;
		FVector FinalLaunchVector = FVector::ZeroVector;
		
		// 카메라 예외
		USpringArmComponent* SpringArmComponent = MyCharacter->GetComponentByClass<USpringArmComponent>();
		FTransform SprintArmComponentTransform;
		if (IsValid(SpringArmComponent))
		{
			SprintArmComponentTransform = SpringArmComponent->GetComponentTransform();
		}

		MyCharacter->SetActorRotation(TargetVector.Rotation());
		MyCharacter->UpdateComponentTransforms();

		// 카메라 예외
		if (IsValid(SpringArmComponent))
		{
			SpringArmComponent->SetWorldTransform(SprintArmComponentTransform);
		}

		UAT_MoveToLocationByVelocity* AbilityTask_MoveToLocation;
		UAT_LaunchCharacterForPeriod* AbilityTask_LaunchCharacter;
		UAT_ChangeCharacterMovementInfo* AbilityTask_ChangeMovementInfo;

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

			AbilityTask_ChangeMovementInfo = UAT_ChangeCharacterMovementInfo::GetNewAbilityTask_ChangeCharacterMovementInfo
			(this, MyCharacter, WalkMovementInfo);
			OnEndAbility.AddUObject(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleEndAbilityTask);
			OnCancelAbility.AddUObject(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleCancelAbilityTask);
			AbilityTask_ChangeMovementInfo->ReadyForActivation();
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
	UAT_WaitGameplayTagAdded* WaitGameplayTagAdded = nullptr;
	UAT_WaitGameplayTagRemoved* WaitGameplayTagRemoved = nullptr;
	UAT_WaitDelay* AbilityTaskDelay = nullptr;

	switch (EndType)
	{

	case ECharacterActionEndType::WaitTagAdded:

		WaitGameplayTagAdded = UAT_WaitGameplayTagAdded::GetNewAbilityTask_WaitGameplayTagAdded(this, EndGameplayTag);
		WaitGameplayTagAdded->Added.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		WaitGameplayTagAdded->ReadyForActivation();

		break;

	case ECharacterActionEndType::WaitTagRemoved:

		WaitGameplayTagRemoved = UAT_WaitGameplayTagRemoved::GetNewAbilityTask_WaitGameplayTagRemoved(this, EndGameplayTag);
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

	// 서버에서 판정 수행
	if (UKismetSystemLibrary::IsServer(GetWorld())
		|| UKismetSystemLibrary::IsStandalone(GetWorld()))
	{
		// 스폰 설정
		UAT_SpawnActors* AbiltiyTaskSpawn = UAT_SpawnActors::GetNewAbilityTask_SpawnActors(this, SpawnActorData);
		AbiltiyTaskSpawn->ReadyForActivation();

		// 공격 설정
		UAT_TraceAttack* AbiltiyTaskAttack = UAT_TraceAttack::GetNewAbilityTask_TraceAttack(this, AttackTraceData);
		AbiltiyTaskAttack->ReadyForActivation();
	}

	// 애니메이션 설정
	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, ActionAnimMontageData);
	OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();
}

bool UGA_CharacterAction::GetIsAbleToActivateCondition()
{
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

	if (MyCharacter == nullptr
		|| MyCharacterMovementComponent == nullptr)
	{
		return false;
	}
	
	return true;
}