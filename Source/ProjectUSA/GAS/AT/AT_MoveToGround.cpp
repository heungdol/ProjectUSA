// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_MoveToGround.h"


#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"


UAT_MoveToGround* UAT_MoveToGround::GetNewAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed)
{
	UAT_MoveToGround* MyObj = NewAbilityTask<UAT_MoveToGround>(OwningAbility, TaskInstanceName);

	MyObj->MoveSpeed = InputMoveSpeed;
	
	MyObj->bIsFinished = false;

	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_MoveToGround::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

}

void UAT_MoveToGround::Activate()
{
	Super::Activate();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_MoveToGround::OnCancelAbilityCallback);
	}

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;
	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		}
	}

	if (CharMoveComp != nullptr)
	{
		CharMoveComp->bOrientRotationToMovement = false;
	}
}

void UAT_MoveToGround::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

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
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGroundReached.Broadcast();
		}

		OnEndAbilityCallback();
	}
	else
	{
		MyCharacterMovementComponent->Velocity = FVector(0, 0, MoveSpeed * -1.0f);
		MyCharacterMovementComponent->UpdateComponentVelocity();
	}
}

void UAT_MoveToGround::OnCancelAbilityCallback()
{
	bIsFinished = true;

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;
	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		}
	}

	if (CharMoveComp != nullptr)
	{
		CharMoveComp->bOrientRotationToMovement = true;
	}

	EndTask();
}

void UAT_MoveToGround::OnEndAbilityCallback()
{
	bIsFinished = true;

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;
	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		}
	}

	if (CharMoveComp != nullptr)
	{
		CharMoveComp->bOrientRotationToMovement = true;
	}

	EndTask();
}
