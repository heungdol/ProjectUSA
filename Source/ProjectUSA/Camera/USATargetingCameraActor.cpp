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

//void AUSATargetingCameraActor::SetOnOff(bool InBool)
//{
//	bIsOn = InBool;
//
//	CalculateTargetingCameraTransform();
//}

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
	//if (bIsOn == false)
	//{
	//	return;
	//}

	if (SourceActor == nullptr
		|| Cast<IUSATargetableInterface>(SourceActor) == nullptr

		|| TargetActor == nullptr
		|| Cast<IUSATargetableInterface>(TargetActor) == nullptr)
	{
		return;
	}

	FVector SourceLocation = Cast<IUSATargetableInterface>(SourceActor)->GetTargetablePivotlocation();
	FVector TargetLocation = Cast<IUSATargetableInterface>(TargetActor)->GetTargetablePivotlocation();

	FVector ResultLocation = ((SourceLocation * LocationWeightRatio) + (TargetLocation * (1.0f - LocationWeightRatio)));
	//ResultLocation.Z = FMath::Max(SourceLocation.Z + ResultLocationHeightOffset, TargetLocation.Z);
	//ResultLocation.Z += SourceHeightOffset;

	//

	float BetweenDistance = (SourceLocation - TargetLocation).Length();

	//

	FVector DirectionFromSourceToTarget = TargetLocation - SourceLocation;
	DirectionFromSourceToTarget.Normalize();

	FVector OffsetRightDirection = FVector::CrossProduct(FVector::UpVector, DirectionFromSourceToTarget);
	FVector OffsetLocation = OffsetRightDirection * RotationDistanceOffset;

	if (BetweenDistance < MinUpdateRange)
	{
		float DistanceOffsetRatio = BetweenDistance / MinUpdateRange;
		OffsetLocation = OffsetRightDirection * RotationDistanceOffset * DistanceOffsetRatio;
	}
	
	//

	//FVector DirectionFromOffsetToSource = SourceLocation - OffsetLocation;`
	//FVector DirectionFromOffsetToTarget = TargetLocation - OffsetLocation;
	//DirectionFromOffsetToSource.Normalize();
	//DirectionFromOffsetToTarget.Normalize();

	//float CurrentTargetHeightOffset = UKismetMathLibrary::MapRangeClamped
	//(TargetActor->GetVelocity().Z, TargetMinVelocityZ, TargetMaxVelocityZ, TargetMinHeightOffset, TargetMaxHeightOffset);
	
	float CurrentTargetHeightOffset = TargetMaxHeightOffset;

	ResultLocation += FVector::UpVector * CurrentTargetHeightOffset + OffsetLocation;

	FVector ResultDirection = (TargetLocation) - ResultLocation;
	ResultDirection.Normalize();

	FRotator ResultRotation = ResultDirection.Rotation();
	ResultRotation.Roll = 0.0f;
	ResultRotation.Pitch = FMath::Clamp(ResultRotation.Pitch, MinRotationPitch, MaxRotationPitch);

	//

	SetActorLocation(ResultLocation);
	
	if (BetweenDistance > MinUpdateRange /** MinUpdateRange*/)
	{
		SetActorRotation(ResultRotation);
	}

	//
	

	float DesiredFOV = UKismetMathLibrary::MapRangeClamped
	(BetweenDistance, MinDistanceForFOV * MinDistanceForFOV, MaxDistanceForFOV * MaxDistanceForFOV, MinFOVOffset, MaxFOVOffset);

	CameraComponent->FieldOfView = DesiredFOV;
}

