// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_MoveToGround.h"


#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Controller.h"

UAT_MoveToGround* UAT_MoveToGround::GetNewAbilityTask
(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed, float InPreDelay, float InPostDelay)
{
	UAT_MoveToGround* MyObj = NewAbilityTask<UAT_MoveToGround>(OwningAbility, TaskInstanceName);

	MyObj->MoveSpeed = InputMoveSpeed;
	MyObj->PreDelay = InPreDelay;
	MyObj->PostDelay = InPostDelay;

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

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* CharMoveComp = nullptr;
	//if (MyActor)
	//{
	//	MyCharacter = Cast<ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	//	}
	//}

	//if (CharMoveComp != nullptr)
	//{
	//	CharMoveComp->bOrientRotationToMovement = false;
	//}

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	//AController* MyController = nullptr;

	//if (MyActor)
	//{
	//	MyCharacter = Cast <ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	//		MyController = MyCharacter->GetController();
	//	}
	//}

	//if (MyActor == nullptr
	//	|| MyCharacter == nullptr
	//	|| MyCharacterMovementComponent == nullptr)
	//{
	//	OnCancelAbilityCallback();

	//	return;
	//}

	//MyController->SetIgnoreMoveInput(true);

	CurrentStep = -1;
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
	
	if (MyActor == nullptr
		|| MyCharacter == nullptr
		|| MyCharacterMovementComponent == nullptr)
	{
		OnCancelAbilityCallback();

		return;
	}

	MyCharacterMovementComponent->Velocity = FVector(0, 0, 0.0f);
	MyCharacterMovementComponent->UpdateComponentVelocity();

	switch (CurrentStep)
	{
	case -1:
		StepPreTime = GetWorld()->GetTimeSeconds();
		StepActiveTime = StepPreTime + PreDelay;
		
		CurrentStep = 0;

		break;

	case 0:
		if (GetWorld()->GetTimeSeconds() >= StepActiveTime)
		{
			OnBeginMovement.Broadcast();

			CurrentStep = 1;
		}

		break;
	case 1:

		if (IsValid(MyCharacterMovementComponent)
			&& MyCharacterMovementComponent->IsFalling() == false)
		{
			OnGroundReached.Broadcast();
			
			StepEndTime = GetWorld()->GetTimeSeconds();
			StepPostTime = StepEndTime + PostDelay;

			CurrentStep = 2;
		}
		else
		{
			MyCharacterMovementComponent->Velocity = FVector(0, 0, MoveSpeed * -1.0f);
			MyCharacterMovementComponent->UpdateComponentVelocity();
		}

		break;

	case 2:

		if (GetWorld()->GetTimeSeconds() > StepPostTime)
		{
			OnEndTask.Broadcast();

			OnEndAbilityCallback();

			CurrentStep = 3;
		}

		break;
	}	
}

void UAT_MoveToGround::OnCancelAbilityCallback()
{
	bIsFinished = true;

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* CharMoveComp = nullptr;
	//if (MyActor)
	//{
	//	MyCharacter = Cast<ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	//	}
	//}

	//if (CharMoveComp != nullptr)
	//{
	//	CharMoveComp->bOrientRotationToMovement = true;
	//}

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	//AController* MyController = nullptr;

	//if (MyActor)
	//{
	//	MyCharacter = Cast <ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	//		MyController = MyCharacter->GetController();
	//	}
	//}

	//if (MyController != nullptr)
	//{
	//	MyController->ResetIgnoreMoveInput();
	//}

	EndTask();
}

void UAT_MoveToGround::OnEndAbilityCallback()
{
	bIsFinished = true;

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* CharMoveComp = nullptr;
	//if (MyActor)
	//{
	//	MyCharacter = Cast<ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	//	}
	//}

	//if (CharMoveComp != nullptr)
	//{
	//	CharMoveComp->bOrientRotationToMovement = true;
	//}

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	//AController* MyController = nullptr;

	//if (MyActor)
	//{
	//	MyCharacter = Cast <ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	//		MyController = MyCharacter->GetController();
	//	}
	//}

	//if (MyController != nullptr)
	//{
	//	MyController->ResetIgnoreMoveInput();
	//}

	EndTask();
}
