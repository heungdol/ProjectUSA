// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_CheckCharacterIsFalling.h"


#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"



UAT_CheckCharacterIsFalling* UAT_CheckCharacterIsFalling::GetNewAbilityTask(UGameplayAbility* OwningAbility, /*ACharacter* InCharacter,*/ UCharacterMovementComponent* InCharacterMovement)
{
	UAT_CheckCharacterIsFalling* MyObj = NewAbilityTask<UAT_CheckCharacterIsFalling>(OwningAbility);
	
	//MyObj->MyCharacter = InCharacter;
	MyObj->MyCharacterMovementComponent = InCharacterMovement;

	MyObj->bTickingTask = true;

	MyObj->bIsCancelable = false;

	return MyObj;
}

void UAT_CheckCharacterIsFalling::Activate()
{
	Super::Activate();

	if (MyCharacterMovementComponent == nullptr)
	{
		BroadcastSimpleDelegate(OnFinished);

		SimpleCancelAbilityTask();
		return;
	}

	bIsFalling = MyCharacterMovementComponent->IsFalling();

	if (bIsFalling)
	{
		if (MyCharacterMovementComponent->Velocity.Z > 1)
		{
			BroadcastSimpleDelegate(OnPositiveFalling);
		}
		else
		{
			BroadcastSimpleDelegate(OnNegativeFalling);
		}
	}
	else
	{
		BroadcastSimpleDelegate(OnGrounded);
	}
}

void UAT_CheckCharacterIsFalling::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MyCharacterMovementComponent == nullptr)
	{
		BroadcastSimpleDelegate(OnFinished);

		SimpleCancelAbilityTask();
		return;
	}

	if (bIsFalling != MyCharacterMovementComponent->IsFalling())	
	{
		bIsFalling = MyCharacterMovementComponent->IsFalling();

		if (bIsFalling)
		{
			if (MyCharacterMovementComponent->Velocity.Z > SMALL_NUMBER)
			{
				BroadcastSimpleDelegate(OnPositiveFalling);
			}
			else
			{
				BroadcastSimpleDelegate(OnNegativeFalling);
			}
		}
		else
		{
			BroadcastSimpleDelegate(OnGrounded);
		}
	}
}
