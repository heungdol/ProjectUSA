// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_InputCharacterMoveForPeriod.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAT_InputCharacterMoveForPeriod* UAT_InputCharacterMoveForPeriod::GetNewAbilityTask
(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, FVector InDirection, float InPeriod)
{
	UAT_InputCharacterMoveForPeriod* MyObj = NewAbilityTask<UAT_InputCharacterMoveForPeriod>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->Direction = InDirection;
	MyObj->Period = InPeriod;

	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_InputCharacterMoveForPeriod::Activate()
{
	Super::Activate();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_InputCharacterMoveForPeriod::OnCancelTaskCallback);
	}

	if (MyCharacter == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	EndTime = GetWorld()->GetTimeSeconds() + Period;
}

void UAT_InputCharacterMoveForPeriod::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MyCharacter == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	FVector InputVector = Direction.X * MyCharacter->GetActorForwardVector()
		+ Direction.Y * MyCharacter->GetActorRightVector();

	MyCharacter->AddMovementInput(InputVector);
	//MyCharacter->GetCharacterMovement()->Velocity

	//UE_LOG(LogTemp, Log, TEXT("Input AbilityTask Tick"));

	if (Period > 0 && EndTime < GetWorld()->GetTimeSeconds())
	{
		OnEndTaskCallback();
	}
}

void UAT_InputCharacterMoveForPeriod::OnCancelTaskCallback()
{
	EndTask();
}

void UAT_InputCharacterMoveForPeriod::OnEndTaskCallback()
{
	EndTask();
}

