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
UAT_MoveToLocationByVelocity* UAT_MoveToLocationByVelocity::GetNewAbilityTask_MoveToLocationByVelocity
(UGameplayAbility* OwningAbility, 
	FName TaskInstanceName, 
	FVector Location, 
	FVector AfterVelocity,
	float Duration, 
	UCurveFloat* OptionalInterpolationCurve, 
	UCurveVector* OptionalVectorInterpolationCurve)
{
	UAT_MoveToLocationByVelocity* MyObj = NewAbilityTask<UAT_MoveToLocationByVelocity>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}

	MyObj->TargetLocation = Location;
	MyObj->AfterVelocity = AfterVelocity;
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
	PrevTime = GetWorld()->GetTimeSeconds();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_MoveToLocationByVelocity::OnCancelTaskCallback);
	}

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;

	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
	}

	if (MyCharacter)
	{
		CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	}

	if (CharMoveComp != nullptr)
	{
		//CharMoveComp->SetMovementMode(EMovementMode::MOVE_None);

		//CharMoveComp->StopActiveMovement();
		//CharMoveComp->Velocity *= -1.0f;
		//CharMoveComp->UpdateProxyAcceleration();
		CharMoveComp->StopMovementImmediately();
	}

	SetWaitingOnAvatar();

	//bIsPassesFirstTick = false;
}

void UAT_MoveToLocationByVelocity::TickTask(float DeltaTime)
{
	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;
	
	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
	}

	if (MyCharacter)
	{
		CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	}

	if (bIsFinished)
	{
		return;
	}

	UGameplayTask::TickTask(DeltaTime);

	if (MyActor)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd
			&& bIsFinished == false)
		{
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


			OffsetLocation = (CurrentLocation - PrevLocation);

			if (OffsetLocation.Z < SMALL_NUMBER
				&& CharMoveComp->IsFalling() == false)
			{
				FVector GroundNormal = MyCharacter->GetCharacterMovement()->CurrentFloor.HitResult.Normal;
				FVector GroundRightVector = FVector::CrossProduct(GroundNormal, FVector::ForwardVector);
				FVector GroundFowardVector = FVector::CrossProduct(GroundRightVector, GroundNormal);

				OffsetLocation = GroundFowardVector * OffsetLocation.X
					+ GroundRightVector * OffsetLocation.Y
					+ GroundNormal * OffsetLocation.Z;
			}

			FVector WorldStartLocation = MyCharacter->GetActorLocation();

			FHitResult Hit;
			MyCharacter->AddActorWorldOffset(OffsetLocation, true, &Hit, ETeleportType::ResetPhysics);

			if (Hit.IsValidBlockingHit() && MyCharacter->GetCharacterMovement()->CurrentFloor.bBlockingHit)
			{
				FVector DeltaLocation = WorldStartLocation + OffsetLocation - MyCharacter->GetActorLocation();

				MyCharacter->AddActorWorldOffset(FVector(0.0f, 0.0f, 10.0f), true, nullptr, ETeleportType::ResetPhysics);

				MyCharacter->AddActorWorldOffset(DeltaLocation, true, &Hit, ETeleportType::ResetPhysics);
			}


			CharMoveComp->Velocity = FVector::ZeroVector;
			CharMoveComp->UpdateComponentVelocity();

			PrevLocation = CurrentLocation;
			
			

			//OffsetLocationDelta = OffsetLocation / DeltaTime;
			//
			//if (CharMoveComp->IsFalling() == false)
			//{
			//	OffsetLocationDelta *= OffsetDeltaNumber;
			//}

			//if (OffsetLocation.Z > SMALL_NUMBER)
			//{
			//	MyCharacter->LaunchCharacter(OffsetLocationDelta, true, true);
			//}
			//else
			//{
			//	CharMoveComp->Velocity = OffsetLocationDelta;
			//	CharMoveComp->UpdateComponentVelocity();
			//}

			//if (CharMoveComp->IsFalling() == false)
			//{

			//}


			//MyCharacter->AddActorWorldOffset(OffsetLocation, true);

			//CharMoveComp->Velocity = FVector::ZeroVector;
			//CharMoveComp->UpdateComponentVelocity()
			// ;
			//MyCharacter->LaunchCharacter(OffsetLocation / DeltaTime, true, true);
		}

		PrevTime = CurrentTime;
	
		//if (bIsPassesFirstTick == false)
		//{
		//	bIsPassesFirstTick = true;
		//}

		if (MyActor->GetLocalRole() == ROLE_Authority)
		{
			MyActor->ForceNetUpdate();
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
	if (bIsFinished == false)
	{
		AActor* MyActor = GetAvatarActor();
		ACharacter* MyCharacter = nullptr;
		UCharacterMovementComponent* CharMoveComp = nullptr;

		if (MyActor)
		{
			MyCharacter = Cast<ACharacter>(MyActor);
		}

		if (MyCharacter != nullptr)
		{
			CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		}

		//

		if (MyCharacter != nullptr)
		{
			OffsetLocation = (TargetLocation - PrevLocation);

			if (OffsetLocation.Z < SMALL_NUMBER
				&& CharMoveComp->IsFalling() == false)
			{
				FVector GroundNormal = MyCharacter->GetCharacterMovement()->CurrentFloor.HitResult.Normal;
				FVector GroundRightVector = FVector::CrossProduct(GroundNormal, FVector::ForwardVector);
				FVector GroundFowardVector = FVector::CrossProduct(GroundRightVector, GroundNormal);

				OffsetLocation = GroundFowardVector * OffsetLocation.X
					+ GroundRightVector * OffsetLocation.Y
					+ GroundNormal * OffsetLocation.Z;
			}

			MyCharacter->AddActorWorldOffset(OffsetLocation, true, nullptr, ETeleportType::ResetPhysics);
		}

		if (CharMoveComp)
		{
			CharMoveComp->Velocity = AfterVelocity;
			CharMoveComp->UpdateComponentVelocity();
		}
	}

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