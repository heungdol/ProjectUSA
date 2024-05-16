// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/USASpringArmComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"


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
	if (bIsUsingCustomLag == false)
	{
		Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
		return;
	}

	ACharacter* Character = Cast<ACharacter> (GetOwner());
	if (Character == nullptr)
	{
		Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
		return;
	}



	bIsCharacterFalling = Character->GetMovementComponent()->IsFalling();
	if (bIsCharacterFalling == false)
	{
		//DesiredCameraZLocation = Character->GetActorLocation().Z;
		DesiredCameraZLocation = Character->GetMesh()->GetComponentLocation().Z;;
	}

	DesiredCameraLocation = Character->GetMesh()->GetComponentLocation();
	DesiredCameraLocation.Z = DesiredCameraZLocation;

	CurrentCameraLocation = FMath::VInterpTo(PrevCameraLocation, DesiredCameraLocation, DeltaTime, CameraInterpSpeed);
	CurrentCameraLocation.Z = FMath::Clamp(CurrentCameraLocation.Z
		, Character->GetMesh()->GetComponentLocation().Z - CameraZUpOffset
		, Character->GetMesh()->GetComponentLocation().Z - CameraZDownOffset);

	//UE_LOG(LogTemp, Log, TEXT("Z: %f"), CurrentCameraLocation.Z);
	
	//SetWorldLocation(CurrentCameraLocation + OffsetLocation);

	//

	PreviousArmOrigin = GetComponentLocation() + TargetOffset;
	PreviousDesiredLoc = CurrentCameraLocation;

	FVector InLocation = CurrentCameraLocation - GetTargetRotation().Vector() * TargetArmLength + FVector::UpVector * OffsetLocation;
	FRotator InRotation = GetTargetRotation();

	FTransform WorldCamTM(InRotation, InLocation);
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();

	PrevCameraLocation = CurrentCameraLocation;

	//

	//bWasCharacterFalling = bIsCharacterFalling;
}
