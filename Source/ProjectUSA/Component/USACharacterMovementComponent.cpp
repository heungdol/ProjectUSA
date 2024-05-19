// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/USACharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "Character/USACharacterBase.h"


float UUSACharacterMovementComponent::GetGravityZ() const
{
	float FinalGravityZ = Super::GetGravityZ();

	if (Velocity.Z > 0)
	{
		FinalGravityZ *= UppingGravityScale;
	}
	else
	{
		FinalGravityZ *= DowningGravityScale;
	}

	return FinalGravityZ;
}

void UUSACharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation))
	{
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		return;
	}

	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

	FRotator DesiredRotation = CurrentRotation;
	FRotator TargetRotation = Velocity.GetSafeNormal().Rotation();

	//AUSACharacterBase* USACharacter = Cast<AUSACharacterBase>(CharacterOwner);

	//if (IsValid(USACharacter))
	//{
	//	TargetRotation = USACharacter->GetUSACharacterDirection_InputMovement().Rotation();
	//}

	float DeltaRotYaw = DeltaRot.Yaw;

	if (bOrientRotationToMovement)
	{
		DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);

		//if (Velocity.SquaredLength() > SMALL_NUMBER)
		//{
		//	if (IsValid(GetCharacterOwner()) == false
		//		|| IsValid(GetCharacterOwner()->GetController<APlayerController>()) == false)
		//	{
		//		DesiredRotation = TargetRotation;
		//	}
		//	else	
		//	{
		//		if (GetCharacterOwner()->GetController<APlayerController>()->IsLocalController() == true)
		//		{
		//			DesiredRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, DeltaRotYaw);
		//		}
		//		else
		//		{
		//			DesiredRotation = TargetRotation;
		//		}
		//	}
		//}
	}
	else if (CharacterOwner->Controller && bUseControllerDesiredRotation)
	{
		DesiredRotation = CharacterOwner->Controller->GetDesiredRotation();
	}
	else if (!CharacterOwner->Controller && bRunPhysicsWithNoController && bUseControllerDesiredRotation)
	{
		if (AController* ControllerOwner = Cast<AController>(CharacterOwner->GetOwner()))
		{
			DesiredRotation = ControllerOwner->GetDesiredRotation();
		}
	}
	else
	{
		return;
	}

	if (ShouldRemainVertical())
	{
		DesiredRotation.Pitch = 0.f;
		DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
		DesiredRotation.Roll = 0.f;
	}
	else
	{
		DesiredRotation.Normalize();
	}

	// Accumulate a desired new rotation.
	const float AngleTolerance = 1e-3f;

	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// PITCH
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
		{
			DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
		}

		// YAW
		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
		}

		// ROLL
		if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
		{
			DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
		}

		// Set the new rotation.
		DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}
