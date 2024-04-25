// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_ChangeCharacterMovementInfo.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"



void FCharacterMovementWalkInfo::RenewCharacterMovementInfo(UCharacterMovementComponent* InMovementComponet)
{
	if (InMovementComponet == nullptr)
	{
		return;
	}

	InMovementComponet->MaxWalkSpeed = FCharacterMovementWalkInfo::MaxWalkSpeed;
	InMovementComponet->RotationRate = FCharacterMovementWalkInfo::RotationRate;
	InMovementComponet->MaxAcceleration = FCharacterMovementWalkInfo::MaxAcceleration;
	InMovementComponet->GroundFriction = FCharacterMovementWalkInfo::GroundFriction;
	InMovementComponet->BrakingDecelerationWalking = FCharacterMovementWalkInfo::BrakingDecelerationWalking;
}

UAT_ChangeCharacterMovementInfo* UAT_ChangeCharacterMovementInfo::GetNewAbilityTask_ChangeCharacterMovementInfo
(UGameplayAbility* OwningAbility, ACharacter* InCharacter, const FCharacterMovementWalkInfo& InInfo)
{
	UAT_ChangeCharacterMovementInfo* MyObj = NewAbilityTask<UAT_ChangeCharacterMovementInfo>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->ChangeCharacterMovementWalkInfo = InInfo;

	MyObj->DefaultCharacterMovementWalkInfo.MaxWalkSpeed
		= InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCharacterMovement()->MaxWalkSpeed;
	MyObj->DefaultCharacterMovementWalkInfo.RotationRate
		= InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCharacterMovement()->RotationRate;
	MyObj->DefaultCharacterMovementWalkInfo.MaxAcceleration
		= InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCharacterMovement()->MaxAcceleration;
	MyObj->DefaultCharacterMovementWalkInfo.GroundFriction
		= InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCharacterMovement()->GroundFriction;
	MyObj->DefaultCharacterMovementWalkInfo.BrakingDecelerationWalking
		= InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCharacterMovement()->BrakingDecelerationWalking;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_ChangeCharacterMovementInfo::SetCharacterMovementInfo()
{
	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		return;
	}

	ChangeCharacterMovementWalkInfo.RenewCharacterMovementInfo(MyCharacter->GetCharacterMovement());
}

void UAT_ChangeCharacterMovementInfo::ResetCharacterMovementInfo()
{
	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		return;
	}

	DefaultCharacterMovementWalkInfo.RenewCharacterMovementInfo(MyCharacter->GetCharacterMovement());
}

void UAT_ChangeCharacterMovementInfo::Activate()
{
	Super::Activate();

	SetCharacterMovementInfo();
}


//void UAT_ChangeCharacterMovementInfo::TickTask(float DeltaTime)
//{
//	Super::TickTask(DeltaTime);
//
//
//
//}
