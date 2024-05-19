// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_MoveToGround.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Controller.h"


UAT_MoveToGround* UAT_MoveToGround::GetNewAbilityTask_MoveToGround
(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed/*, float InPreDelay, float InPostDela*/)
{
	UAT_MoveToGround* MyObj = NewAbilityTask<UAT_MoveToGround>(OwningAbility, TaskInstanceName);

	MyObj->MoveSpeed = InputMoveSpeed;

	MyObj->bIsFinished = false;

	MyObj->bTickingTask = true;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_MoveToGround::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

}

void UAT_MoveToGround::Activate()
{
	Super::Activate();

	//if (AbilitySystemComponent.IsValid())
	//{
	//	AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_MoveToGround::SimpleCancelAbilityTask);
	//}

	BroadcastSimpleDelegate(OnBeginMovement);

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (MyActor)
	{
		MyCharacter = Cast <ACharacter>(MyActor);
		if (MyCharacter)
		{
			MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
		}
	}

	if (MyActor == nullptr
		|| MyCharacter == nullptr
		|| MyCharacterMovementComponent == nullptr)
	{
		SimpleCancelAbilityTask();

		return;
	}

	//else
	{
		//MyCharacterMovementComponent->Velocity = FVector(0, 0, MoveSpeed * -1.0f);
		//MyCharacterMovementComponent->UpdateComponentVelocity();
		MyCharacter->LaunchCharacter(FVector(0, 0, MoveSpeed * -1.0f), true, true);
	}
}

void UAT_MoveToGround::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (bIsFinished)
	{
		return;
	}

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (MyActor)
	{
		MyCharacter = Cast <ACharacter>(MyActor);
		if (MyCharacter)
		{
			MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
		}
	}

	if (IsValid(MyCharacterMovementComponent)
		&& MyCharacterMovementComponent->IsFalling() == false)
	{
		BroadcastSimpleDelegate(OnGroundReached);

		SimpleEndAbilityTask();
	}
}

void UAT_MoveToGround::SimpleCancelAbilityTask()
{
	bIsFinished = true;
	
	Super::SimpleCancelAbilityTask();
}

void UAT_MoveToGround::SimpleEndAbilityTask()
{
	bIsFinished = true;

	Super::SimpleEndAbilityTask();
}
