// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_MoveToLocationByVelocity.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#include "AbilitySystemComponent.h"


// TODO: 공중 로테이션 다듬어 볼 것
UAT_MoveToLocationByVelocity* UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve)
{
	UAT_MoveToLocationByVelocity* MyObj = NewAbilityTask<UAT_MoveToLocationByVelocity>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}

	MyObj->TargetLocation = Location;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
	MyObj->LerpCurve = OptionalInterpolationCurve;
	MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

	MyObj->bTickingTask = true;
	MyObj->bSimulatedTask = true;
	MyObj->bIsFinished = false;

	return MyObj;
}

void UAT_MoveToLocationByVelocity::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);


}

void UAT_MoveToLocationByVelocity::Activate()
{
	Super::Activate();

	PrevLocation = StartLocation;

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_MoveToLocationByVelocity::OnCancelTaskCallback);
	}
}

void UAT_MoveToLocationByVelocity::TickTask(float DeltaTime)
{
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

	if (bIsFinished)
	{
		return;
	}

	UGameplayTask::TickTask(DeltaTime);

	if (MyActor)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			FVector OffsetLocation = (TargetLocation - PrevLocation);
			CharMoveComp->Velocity = OffsetLocation / DeltaTime;
			CharMoveComp->UpdateComponentVelocity();

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnTargetLocationReached.Broadcast();
			}

			if (MyActor->GetLocalRole() == ROLE_Authority)
			{
				MyActor->ForceNetUpdate();
			}

			OnEndTaskCallback();
		}
		else
		{
			FVector CurrentLocation;

			float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
			if (LerpCurveVector)
			{
				const FVector ComponentInterpolationFraction = LerpCurveVector->GetVectorValue(MoveFraction);
				CurrentLocation = FMath::Lerp<FVector, FVector>(StartLocation, TargetLocation, ComponentInterpolationFraction);
			}
			else
			{
				if (LerpCurve)
				{
					MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
				}

				CurrentLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);
			}

			FVector OffsetLocation = (CurrentLocation - PrevLocation);
			PrevLocation = CurrentLocation;

			CharMoveComp->Velocity = OffsetLocation / DeltaTime;
			CharMoveComp->UpdateComponentVelocity();
		}
	}
	else
	{
		OnEndTaskCallback();
	}
}

void UAT_MoveToLocationByVelocity::OnCancelTaskCallback()
{
	bIsFinished = true;

	ExternalCancel();
}

void UAT_MoveToLocationByVelocity::OnEndTaskCallback()
{
	bIsFinished = true;

	EndTask();
}

void UAT_MoveToLocationByVelocity::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, StartLocation);
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, TargetLocation);
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, PrevLocation);
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, DurationOfMovement);
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, LerpCurve);
	DOREPLIFETIME(UAT_MoveToLocationByVelocity, LerpCurveVector);
}