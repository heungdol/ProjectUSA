// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/USACameraComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

void UUSACameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUSACameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	if (bFOVOffsetByCharacterMovement)
	{
		if (bUsingXYVelocity == false
			&& bUsingZVelocity == false)
		{
			return;
		}

		ACharacter* Character = Cast <ACharacter>(GetOwner());
		if (Character == nullptr)
		{
			return;
		}

		FVector CurrentCharacterVelocity = Character->GetVelocity();

		if (bUsingXYVelocity == false)
		{
			CurrentCharacterVelocity.X = 0.0f;
			CurrentCharacterVelocity.Y = 0.0f;
		}

		if (bUsingZVelocity == false)
		{
			CurrentCharacterVelocity.Z = 0.0f;
		}

		float CurrentCharacterVelocityLenght = CurrentCharacterVelocity.Length();

		DesiredFOVOffset = UKismetMathLibrary::MapRangeClamped
		(CurrentCharacterVelocityLenght, 0, CharacterMovespeedMax, 0, FOVOffsetMax);

		CurrentFOVOffset = FMath::FInterpTo(PrevFOVOffset, DesiredFOVOffset, DeltaTime, FOVOffsetInterpSpeed);
		PrevFOVOffset = CurrentFOVOffset;

		DesiredView.FOV = DesiredView.FOV + CurrentFOVOffset;
	}
}
