// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CapsuleComponent.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"
#include "GAS/AT/AT_LaunchCharacterForPeriod.h"
#include "GAS/AT/AT_WaitDelay.h"
#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_WaitGameplayTag.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

//#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "Interface/USAAttackableInterface.h"
#include "Interface/USATargetableInterface.h"

#include "Character/USACharacterBase.h"

#include "Kismet/KismetSystemLibrary.h"

#include "AbilitySystemComponent.h"

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

	// 무기 어태치 수행
	//if (bHasToAttachedAllWeaponToHolder == true)
	//{
	//	AUSACharacterBase* MyUSACharacter = nullptr;
	//	UCharacterMovementComponent* MyCharacaterMovementComponent = nullptr;

	//	if (CurrentActorInfo != nullptr)
	//	{
	//		MyUSACharacter = Cast <AUSACharacterBase>(CurrentActorInfo->AvatarActor);
	//	}

	//	if (MyUSACharacter != nullptr)
	//	{
	//		MyUSACharacter->AttachAllWeaponToHolderSocket();
	//	}
	//}

	// Target Vector 활용
	ActivateAbilityUsingTargetVector(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 어트리뷰트 수행
	AddArmorAttributeFromBase(ArmorAttributeAddNumber);

	// 인터럽트 딜레이 체크 수행
	//bIsInterruptDelayOver = false;
	//bIsInterruptCondtion = false;

	//if (InterruptDelay < SMALL_NUMBER)
	//{
	//	//SetInterruptDelayOver();
	//}
	//else
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(TimerHandleInterruptDelay);
	//	GetWorld()->GetTimerManager().SetTimer(TimerHandleInterruptDelay, FTimerDelegate::CreateLambda([=]()
	//		{
	//			SetInterruptDelayOver();
	//		}
	//	), InterruptDelay, false);

		// 인터럽트 첫 체크
		if (InterruptGameplayTag.IsValid() == true
			&& IsValid(GetAbilitySystemComponentFromActorInfo()) == true)
		{
			switch (InterruptType)
			{

			case ECharacterActionEndType::WaitTagAdded:

				if (GetAbilitySystemComponentFromActorInfo()->GetGameplayTagCount(InterruptGameplayTag) > 0)
				{
					SimpleEndAbility();
				}

				break;

			case ECharacterActionEndType::WaitTagRemoved:

				if (GetAbilitySystemComponentFromActorInfo()->GetGameplayTagCount(InterruptGameplayTag) <= 0)
				{
					SimpleEndAbility();
				}

				break;

			case ECharacterActionEndType::WaitTime:
			default:

				break;

			}
		}
	//}

	K2_DoSomething_Activate();
}

void UGA_CharacterAction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	// 어트리뷰트 종료
	ResetArmorAttributeToBase();

	// 딜레이 종료
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandleInterruptDelay);
	//TimerHandleInterruptDelay.Invalidate();

	K2_DoSomething_Cancel();
}

void UGA_CharacterAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 어트리뷰트 종료
	ResetArmorAttributeToBase();

	// 딜레이 종료
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandleInterruptDelay);
	//TimerHandleInterruptDelay.Invalidate();

	K2_DoSomething_End();
}

void UGA_CharacterAction::CalculateTargetVector()
{
	ACharacter* MyCharacter = nullptr;
	AUSACharacterBase* MyUSACharacter = nullptr;
	UCharacterMovementComponent* MyCharacaterMovementComponent = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyUSACharacter = Cast <AUSACharacterBase>(MyCharacter);

		MyCharacaterMovementComponent = MyCharacter->GetCharacterMovement();

		TargetVector_Move = MyCharacter->GetActorForwardVector();
		TargetVector_Attack = FVector::ZeroVector;
	}

	// 액션 수행 전마다 임시 타겟팅 업데이트
	if (MyUSACharacter)
	{	
		MyUSACharacter->UpdateCurrentTargetableActor_Instant();
	}

	bool bIsFinalMoveToTargetAction = false;
	float TempDistance = -1.0f;
	FVector TempVector = MyCharacter->GetActorForwardVector();

	IUSAAttackableInterface* AttackableInterface = Cast<IUSAAttackableInterface>(GetAvatarActorFromActorInfo());
	IUSATargetableInterface* TargetableActorInterface = nullptr;

	//
	
	if (AttackableInterface != nullptr
		&& AttackableInterface->GetIsTargeting() == true)
	{
		TargetableActorInterface = AttackableInterface->GetTargetableInterface();

		FVector TargetableActorLocation = AttackableInterface->GetTargetableActorLocation();

		TargetVector_Attack = (TargetableActorLocation - MyCharacter->GetActorLocation());
		TargetVector_Attack.Normalize();

		//

		if (IsValid(MyCharacaterMovementComponent) == true
			&& MyCharacaterMovementComponent->IsFalling() == false)
		{
			TargetableActorLocation.Z = MyCharacter->GetActorLocation().Z;
		}

		TempVector = (TargetableActorLocation - MyCharacter->GetActorLocation());
		TempVector.Normalize();

		TempDistance = (TargetableActorLocation - MyCharacter->GetActorLocation()).Length();

		bIsFinalMoveToTargetAction = (TempDistance <= MoveToTargetRange);

	}
	
	//

	if (bIsMoveToTargetAction == true && bIsFinalMoveToTargetAction == true)
	{
		float TargetRadius = 0.0f;
		if (TargetableActorInterface != nullptr)
		{
			TargetRadius = TargetableActorInterface->GetTargetableCapsuleRadius();
		}

		float SourceRadius = 0.0f;
		if (IsValid(MyCharacter) == true)
		{
			SourceRadius = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		}

		TargetDistance = FMath::Max(TempDistance - TargetRadius - SourceRadius - MoveToTargetGap, 0.0f);
		TargetVector_Move = TempVector;
	}
	else
	{
		TargetDistance = -1.0f;

		switch (DirectionType)
		{
		case ECharacterActionDirectionType::Input:

			if (MyUSACharacter != nullptr
				&& MyUSACharacter->GetUSACharacterDirection_InputMovement().SquaredLength() > SMALL_NUMBER)
			{
				TargetVector_Move = MyUSACharacter->GetUSACharacterDirection_InputMovement();
				//TargetVector_Attack = TargetVector_Move;
			}
			else if (MyCharacter != nullptr
				&& MyCharacter->GetPendingMovementInputVector().SquaredLength() > SMALL_NUMBER)
			{
				TargetVector_Move = MyCharacter->GetPendingMovementInputVector();
				//TargetVector_Attack = TargetVector_Move;
			}
			else
			{
				TargetVector_Move = MyCharacter->GetActorForwardVector();
				//TargetVector_Attack = TargetVector_Move;
			}

			break;

		case ECharacterActionDirectionType::Target:
			if (MyUSACharacter != nullptr
				&& MyUSACharacter->GetUSACharacterDirection_Target().SquaredLength() > SMALL_NUMBER)
			{
				TargetVector_Move = MyUSACharacter->GetUSACharacterDirection_Target();
				//TargetVector_Attack = TempVector;
			}
			else if (MyUSACharacter != nullptr
				&& MyUSACharacter->GetUSACharacterDirection_InputMovement().SquaredLength() > SMALL_NUMBER)
			{
				TargetVector_Move = MyUSACharacter->GetUSACharacterDirection_InputMovement();
				//TargetVector_Attack = TargetVector_Move;
			}
			else if (MyCharacter != nullptr
				&& MyCharacter->GetPendingMovementInputVector().SquaredLength() > SMALL_NUMBER)
			{
				TargetVector_Move = MyCharacter->GetPendingMovementInputVector();
				//TargetVector_Attack = TargetVector_Move;
			}
			else
			{
				TargetVector_Move = MyCharacter->GetActorForwardVector();
				//TargetVector_Attack = TargetVector_Move;
			}

			break;

		case ECharacterActionDirectionType::None:
		default:


			break;
		}
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
	IUSAAttackableInterface* AttackableInterface = nullptr;

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
		FVector	ForwardDirection = TargetVector_Move;
		ForwardDirection.Z = 0.0f;
		ForwardDirection.Normalize();

		FVector	RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
		RightDirection.Normalize();

		//

		FVector EndLocation = FVector::ZeroVector;
		FVector AfterVelocity = FVector::ZeroVector;
		FVector FinalLaunchVector = FVector::ZeroVector;
		
		//

		// 카메라 예외
		USpringArmComponent* SpringArmComponent = MyCharacter->GetComponentByClass<USpringArmComponent>();
		FTransform SprintArmComponentTransform;
		if (IsValid(SpringArmComponent))
		{
			SprintArmComponentTransform = SpringArmComponent->GetComponentTransform();
		}

		//

		MyCharacter->SetActorRotation(ForwardDirection.Rotation());
		MyCharacter->UpdateComponentTransforms();

		//

		// 카메라 예외
		if (IsValid(SpringArmComponent))
		{
			SpringArmComponent->SetWorldTransform(SprintArmComponentTransform);
		}

		//

		UAT_MoveToLocationByVelocity* AbilityTask_MoveToLocation;
		UAT_LaunchCharacterForPeriod* AbilityTask_LaunchCharacter;
		UAT_ChangeCharacterMovementInfo* AbilityTask_ChangeMovementInfo;

		//

		//bool bIsFinalMoveToTargetAction = false;
		//float BetweenDistance = 0.0f;

		//AttackableInterface = Cast<IUSAAttackableInterface>(GetAvatarActorFromActorInfo());

		//if (bIsMoveToTargetAction)
		//{
		//	if (AttackableInterface != nullptr
		//		&& AttackableInterface->GetIsTargeting())
		//	{
		//		FVector TargetableActorLocation = AttackableInterface->GetTargetableActorLocation();
		//		TargetableActorLocation.Z = MyCharacter->GetActorLocation().Z;

		//		BetweenDistance = (TargetableActorLocation - MyCharacter->GetActorLocation()).Length();
		//		if (BetweenDistance < MoveToTargetRange)
		//		{
		//			bIsFinalMoveToTargetAction = true;
		//		}
		//	}
		//}


		if (TargetDistance > -SMALL_NUMBER)
		{
			//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("B"));
			//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("%f %f %f"), EndMoveDistance, TargetRadius, SourceRadius);

			EndLocation = MyCharacter->GetActorLocation() + TargetVector_Move * TargetDistance;

			AfterVelocity = (ForwardDirection * MoveToTargetAfterVelocity.X)
				+ (RightDirection * MoveToTargetAfterVelocity.Y)
				+ (FVector::UpVector * MoveToTargetAfterVelocity.Z);

			AbilityTask_MoveToLocation = UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity
			(this, TEXT("MoveToTarget"), EndLocation, AfterVelocity, MoveToTargetDuration, MoveToTargetCurveFloat, MoveToTargetCurveVector);

			AbilityTask_MoveToLocation->ReadyForActivation();
		}
		else
		{
			switch (MoveType)
			{
			case ECharacterActionMoveType::Move:
				EndLocation = MyCharacter->GetActorLocation()
					+ (ForwardDirection * MoveOffsetLocation.X)
					+ (RightDirection * MoveOffsetLocation.Y)
					+ (FVector::UpVector * MoveOffsetLocation.Z);

				AfterVelocity = (ForwardDirection * MoveAfterVelocity.X)
					+ (RightDirection * MoveAfterVelocity.Y)
					+ (FVector::UpVector * MoveAfterVelocity.Z);

				AbilityTask_MoveToLocation = UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity
				(this, TEXT("Move"), EndLocation, AfterVelocity, MoveDuration, MoveCurveFloat, nullptr);

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

	// 인터럽트 조건 설정
	UAT_WaitGameplayTagAdded* WaitGameplayTagAdded_Interrupt = nullptr;
	UAT_WaitGameplayTagRemoved* WaitGameplayTagRemoved_Interrupt = nullptr;

	switch (InterruptType)
	{

	case ECharacterActionEndType::WaitTagAdded:

		WaitGameplayTagAdded_Interrupt = UAT_WaitGameplayTagAdded::GetNewAbilityTask_WaitGameplayTagAdded(this, InterruptGameplayTag);
		WaitGameplayTagAdded_Interrupt->Added.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		WaitGameplayTagAdded_Interrupt->ReadyForActivation();

		break;

	case ECharacterActionEndType::WaitTagRemoved:

		WaitGameplayTagRemoved_Interrupt = UAT_WaitGameplayTagRemoved::GetNewAbilityTask_WaitGameplayTagRemoved(this, InterruptGameplayTag);
		WaitGameplayTagRemoved_Interrupt->Removed.AddDynamic(this, &UGA_CharacterAction::SimpleEndAbility);
		WaitGameplayTagRemoved_Interrupt->ReadyForActivation();

		break;

	case ECharacterActionEndType::WaitTime:
	default:

		break;

	}

	// 서버에서 판정 수행
	if (UKismetSystemLibrary::IsServer(GetWorld())
		|| UKismetSystemLibrary::IsStandalone(GetWorld()))
	{
		// 스폰 설정
		UAT_SpawnActors* AbiltiyTaskSpawn = UAT_SpawnActors::GetNewAbilityTask_SpawnActors(this, SpawnActorData, TargetVector_Attack);
		AbiltiyTaskSpawn->ReadyForActivation();

		// 공격 설정
		UAT_TraceAttack* AbiltiyTaskAttack = UAT_TraceAttack::GetNewAbilityTask_TraceAttack(this, AttackTraceData, TargetVector_Attack);
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

void UGA_CharacterAction::AddArmorAttributeFromBase(float InAddArmor)
{
	if (bIsAppliedArmorAttribute == false)
	{
		bIsAppliedArmorAttribute = true;

		UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
		if (OwnerASC != nullptr
			&& OwnerASC->GetSet <UUSAAttributeSet>() != nullptr)
		{
			float BaseArmor = 0.0f;
			bool CheckIsAttributeFound = false;
			BaseArmor = OwnerASC->GetGameplayAttributeValue(UUSAAttributeSet::GetBaseArmorAttribute(), CheckIsAttributeFound);

			if (CheckIsAttributeFound == true)
			{
				OwnerASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentArmorAttribute(), BaseArmor + InAddArmor);
			}
		}
	}
}

void UGA_CharacterAction::ResetArmorAttributeToBase()
{
	if (bIsAppliedArmorAttribute == true)
	{
		bIsAppliedArmorAttribute = false;

		UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
		if (OwnerASC != nullptr
			&& OwnerASC->GetSet <UUSAAttributeSet>() != nullptr)
		{
			float BaseArmor = 0.0f;
			bool CheckIsAttributeFound = false;
			BaseArmor = OwnerASC->GetGameplayAttributeValue(UUSAAttributeSet::GetBaseArmorAttribute(), CheckIsAttributeFound);

			if (CheckIsAttributeFound == true)
			{
				OwnerASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentArmorAttribute(), BaseArmor);
			}
		}
	}
}

//void UGA_CharacterAction::SetInterruptDelayOver()
//{
//	bIsInterruptDelayOver = true;
//
//	if (bIsInterruptCondtion == true && bIsInterruptDelayOver == true)
//	{
//		SimpleEndAbility();
//	}
//}
//
//void UGA_CharacterAction::SetInterruptCondition()
//{
//	bIsInterruptCondtion = true;
//
//	if (bIsInterruptCondtion == true && bIsInterruptDelayOver == true)
//	{
//		SimpleEndAbility();
//	}
//}
