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

	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	FVector DesiredLoc = CurrentCameraLocation - GetTargetRotation().Vector() * TargetArmLength + FVector::UpVector * OffsetLocation;

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	FVector ResultLoc;
	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());

		FHitResult Result;
		GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		UnfixedCameraPosition = DesiredLoc;

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

		if (ResultLoc == DesiredLoc)
		{
			bIsCameraFixed = false;
		}
	}
	else
	{
		ResultLoc = DesiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLoc;
	}

	//


	// Form a transform for new world transform for camera
	FTransform WorldCamTM(GetTargetRotation(), ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();

	PrevCameraLocation = CurrentCameraLocation;

	//

	//bWasCharacterFalling = bIsCharacterFalling;
}
