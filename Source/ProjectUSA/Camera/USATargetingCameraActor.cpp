// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/USATargetingCameraActor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

AUSATargetingCameraActor::AUSATargetingCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject <USpringArmComponent>(TEXT("Camera Spring Arm Component"));
	CameraSpringArmComponent->TargetArmLength = 500.0f;
	
	RootComponent = CameraSpringArmComponent;

	CameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);
}

// Called when the game starts or when spawned
void AUSATargetingCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSATargetingCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateTargetingCameraTransform();
}

void AUSATargetingCameraActor::SetOnOff(bool InBool)
{
	bIsOn = InBool;

	CalculateTargetingCameraTransform();
}

void AUSATargetingCameraActor::SetSourceActor(AActor* InActor)
{
	SourceActor = InActor;
}

void AUSATargetingCameraActor::SetTargetActor(AActor* InActor)
{
	TargetActor = InActor;
}

void AUSATargetingCameraActor::CalculateTargetingCameraTransform()
{
	if (bIsOn == false)
	{
		return;
	}

	if (SourceActor == nullptr
		|| TargetActor == nullptr)
	{
		return;
	}

	FVector SourceLocation = SourceActor->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	FVector ResultLocation = ((SourceLocation * LocationWeightRatio) + (TargetLocation * (1.0f - LocationWeightRatio)));
	ResultLocation.Z = TargetLocation.Z;/*FMath::Min(SourceLocation.Z, TargetLocation.Z);*/
	ResultLocation.Z += LocationHeightOffset;

	//

	FVector DirectionFromSourceToTarget = TargetLocation - SourceLocation;
	DirectionFromSourceToTarget.Normalize();

	FVector OffsetDirection = FVector::CrossProduct(FVector::UpVector, DirectionFromSourceToTarget);
	FVector OffsetLocation = ResultLocation + OffsetDirection * RotationDistanceOffset;
	
	//

	//FVector DirectionFromOffsetToSource = SourceLocation - OffsetLocation;
	//FVector DirectionFromOffsetToTarget = TargetLocation - OffsetLocation;
	//DirectionFromOffsetToSource.Normalize();
	//DirectionFromOffsetToTarget.Normalize();

	FVector ResultDirection = TargetLocation - OffsetLocation;
	ResultDirection.Normalize();

	FRotator ResultRotation = ResultDirection.Rotation();
	ResultRotation.Roll = 0.0f;

	//

	SetActorLocation(ResultLocation);
	SetActorRotation(ResultRotation);

	//

	float BetweenDistance = (SourceLocation - TargetLocation).Length();

	float DesiredFOV = UKismetMathLibrary::MapRangeClamped
	(BetweenDistance, MinDistanceForFOV, MaxDistanceForFOV, MinFOVOffset, MaxFOVOffset);

	CameraComponent->FieldOfView = DesiredFOV;
}

