// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_MaintainCharacterVelocity.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Controller.h"


UAT_MaintainCharacterVelocity* UAT_MaintainCharacterVelocity::GetNewAbilityTask
(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InVelocitySpeed, bool InIsConsiderZVelocity)
{
    UAT_MaintainCharacterVelocity* MyObj = NewAbilityTask<UAT_MaintainCharacterVelocity>(OwningAbility, TaskInstanceName);

	MyObj->VelocitySpeed = InVelocitySpeed;
	MyObj->bIsConsideringZVelocity = InIsConsiderZVelocity;

    MyObj->bTickingTask = true;

	MyObj->bIsCancelable = true;

    return MyObj;
}


void UAT_MaintainCharacterVelocity::Activate()
{
    Super::Activate();

	SetWaitingOnAvatar();
}

void UAT_MaintainCharacterVelocity::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (MyActor)
	{
		MyCharacter = Cast <ACharacter>(MyActor);
	}

	if (MyCharacter)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}
	

	if (MyCharacter == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	if (MyCharacterMovementComponent == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}


	FVector VelocityDirection = MyCharacter->GetActorForwardVector(); /*MyCharacter->GetVelocity()*/;

	if (bIsConsideringZVelocity == false)
	{ 
		VelocityDirection.Z = 0.0f;
	}

	VelocityDirection.Normalize();

	MyCharacterMovementComponent->Velocity = VelocityDirection * VelocitySpeed;
	MyCharacterMovementComponent->UpdateComponentVelocity();
}
