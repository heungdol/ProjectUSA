// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/USACharacterMovementComponent.h"

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
