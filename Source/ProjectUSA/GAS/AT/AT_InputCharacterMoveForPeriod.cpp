// Fill out your copyright notice in the Description page of Project Settings.


//#include "GAS/AT/AT_InputCharacterMoveForPeriod.h"
//
//#include "AbilitySystemComponent.h"
//
//#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"
//
//UAT_InputCharacterMoveForPeriod* UAT_InputCharacterMoveForPeriod::GetNewAbilityTask_InputCharacterMoveForPeriod
//(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, FVector InDirection, float InPeriod)
//{
//	UAT_InputCharacterMoveForPeriod* MyObj = NewAbilityTask<UAT_InputCharacterMoveForPeriod>(OwningAbility);
//
//	MyObj->MyCharacter = InCharacter;
//	MyObj->Direction = InDirection;
//	MyObj->Period = InPeriod;
//
//	MyObj->bTickingTask = true;
//
//	MyObj->bIsCancelable = true;
//
//	return MyObj;
//}
//
//void UAT_InputCharacterMoveForPeriod::Activate()
//{
//	Super::Activate();
//
//	if (MyCharacter == nullptr)
//	{
//		SimpleCancelAbilityTask();
//		return;
//	}
//
//	if (MyCharacter->GetCharacterMovement() == nullptr)
//	{
//		SimpleCancelAbilityTask();
//		return;
//	}
//
//	EndTime = GetWorld()->GetTimeSeconds() + Period;
//
//	SetWaitingOnAvatar();
//}
//
//void UAT_InputCharacterMoveForPeriod::TickTask(float DeltaTime)
//{
//	Super::TickTask(DeltaTime);
//
//	if (MyCharacter == nullptr)
//	{
//		SimpleCancelAbilityTask();
//		return;
//	}
//
//	if (MyCharacter->GetCharacterMovement() == nullptr)
//	{
//		SimpleCancelAbilityTask();
//		return;
//	}
//
//	FVector InputVector = Direction.X * MyCharacter->GetActorForwardVector()
//		+ Direction.Y * MyCharacter->GetActorRightVector();
//
//	MyCharacter->AddMovementInput(InputVector);
//
//	if (Period > 0 && EndTime < GetWorld()->GetTimeSeconds())
//	{
//		SimpleEndAbilityTask();
//	}
//}