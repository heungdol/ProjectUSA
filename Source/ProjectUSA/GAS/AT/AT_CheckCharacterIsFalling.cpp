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

	return MyObj;
}

void UAT_CheckCharacterIsFalling::Activate()
{
	Super::Activate();

	//UE_LOG(LogTemp, Log, TEXT("Abiltiy Task Activate"));

	if (MyCharacterMovementComponent == nullptr)
	{
		OnFinished.Broadcast();

		EndTask();
	}

		bIsFalling = MyCharacterMovementComponent->IsFalling();

		if (bIsFalling)
		{
			if (MyCharacterMovementComponent->Velocity.Z > 1)
			{
				OnPositiveFalling.Broadcast();
			}
			else
			{
				OnNegativeFalling.Broadcast();
			}
		}
		else
		{
			OnGrounded.Broadcast();
		}
}

void UAT_CheckCharacterIsFalling::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Abiltiy Task Tick"));

	if (MyCharacterMovementComponent == nullptr)
	{
		OnFinished.Broadcast();

		EndTask();
	}

	if (bIsFalling != MyCharacterMovementComponent->IsFalling())	
	{
		bIsFalling = MyCharacterMovementComponent->IsFalling();

		if (bIsFalling)
		{
			if (MyCharacterMovementComponent->Velocity.Z > SMALL_NUMBER)
			{
				//UE_LOG(LogTemp, Log, TEXT("Positive Falling"));

				OnPositiveFalling.Broadcast();
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("Negative Falling"));

				OnNegativeFalling.Broadcast();
			}
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Grounded"));

			OnGrounded.Broadcast();
		}
	}
}
