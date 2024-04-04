// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_MoveToGround.h"


#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// TODO: Cancel 된 경우도 처리할 것


UAT_MoveToGround* UAT_MoveToGround::GetNewAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed)
{
	UAT_MoveToGround* MyObj = NewAbilityTask<UAT_MoveToGround>(OwningAbility, TaskInstanceName);

	MyObj->MoveSpeed = InputMoveSpeed;
	//MyObj->BouncePower = InputBouncePower;

	MyObj->bTickingTask = true;
	MyObj->bSimulatedTask = true;
	MyObj->bIsFinished = false;

	return MyObj;
}

void UAT_MoveToGround::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

}

void UAT_MoveToGround::Activate()
{
	Super::Activate();

}

void UAT_MoveToGround::TickTask(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("Move to Ground Task Tick... Pre"));

	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	//UE_LOG(LogTemp, Log, TEXT("Move to Ground Task Tick..."));

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
		bIsFinished = true;

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGrounded.Broadcast();
		}

		EndTask();
	}
	else
	{
		MyCharacterMovementComponent->Velocity = FVector(0, 0, MoveSpeed * -1.0f);
		MyCharacterMovementComponent->UpdateComponentVelocity();
	}

}
