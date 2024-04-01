// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/USASpringArmComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UUSASpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	if (Actor != nullptr)
	{
		DesiredCameraZLocation = Actor->GetActorLocation().Z;

		DesiredCameraLocation
			= CurrentCameraLocation
			= PrevCameraLocation
			= Actor->GetActorLocation();
	}
}

void UUSASpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);

	if (bIsUsingCustomLag == false)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter> (GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	bIsCharacterFalling = Character->GetMovementComponent()->IsFalling();
	if (bIsCharacterFalling == false)
	{
		DesiredCameraZLocation = Character->GetActorLocation().Z;
	}

	DesiredCameraLocation = Character->GetActorLocation();
	DesiredCameraLocation.Z = DesiredCameraZLocation;

	CurrentCameraLocation = FMath::VInterpTo(PrevCameraLocation, DesiredCameraLocation, DeltaTime, CameraInterpSpeed);
	CurrentCameraLocation.Z = FMath::Clamp(CurrentCameraLocation.Z, Character->GetActorLocation().Z - CameraZUpOffset, Character->GetActorLocation().Z - CameraZDownOffset);
	
	SetWorldLocation(CurrentCameraLocation + OffsetLocation);
	PrevCameraLocation = CurrentCameraLocation;
	//bWasCharacterFalling = bIsCharacterFalling;
}
