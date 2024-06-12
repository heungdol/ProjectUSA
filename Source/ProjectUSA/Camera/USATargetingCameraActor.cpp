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
	
	//RootComponent = CameraSpringArmComponent;

	CameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	CameraFocusStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Camera Target Static Mesh Component"));
	CameraFocusStaticMeshComponent->SetupAttachment(RootComponent);
	CameraFocusStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	CameraFocusStaticMeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void AUSATargetingCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (CameraFocusStaticMeshComponent)
	{
		CameraFocusStaticMeshComponent->bHiddenInGame = true;
	}
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

	if (CameraFocusStaticMeshComponent && InActor == nullptr)
	{
		CameraFocusStaticMeshComponent->bHiddenInGame = true;
	}
}

void AUSATargetingCameraActor::CalculateTargetingCameraTransform()
{
	//if (bIsOn == false)
	//{
	//	return;
	//}

	if (SourceActor == nullptr
		|| Cast<IUSATargetableInterface>(SourceActor) == nullptr)
	{
		return;
	}

	if (TargetActor == nullptr
		|| Cast<IUSATargetableInterface>(TargetActor) == nullptr)
	{
		SetActorLocation(SourceActor->GetActorLocation());
		SetActorRotation(SourceActor->GetActorForwardVector().Rotation());

		return;
	}

	FVector CameraLocation = CameraComponent->GetComponentLocation();

	FVector SourceLocation = Cast<IUSATargetableInterface>(SourceActor)->GetTargetablePivotlocation();
	FVector TargetLocation = Cast<IUSATargetableInterface>(TargetActor)->GetTargetablePivotlocation();

	//

	FVector ResultLocation = ((SourceLocation * LocationWeightRatio) + (TargetLocation * (1.0f - LocationWeightRatio)));
	ResultLocation.Z += OffsetResultLocationHeight;

	FVector ResultDirection = TargetLocation - SourceLocation;
	ResultDirection.Normalize();

	SetActorLocation(ResultLocation);

	//

	float BetweenDistance = (SourceLocation - TargetLocation).Length();

	//if (BetweenDistance > MinUpdateRange /** MinUpdateRange*/)
	{
		FVector ResultDirection2D = TargetLocation - SourceLocation;
		ResultDirection2D.Z = 0.0f;

		FRotator ResultRotator = ResultDirection2D.Rotation();
		ResultRotator.Yaw += OffsetRotationYaw;
		ResultRotator.Pitch = FMath::Clamp(ResultDirection.Rotation().Pitch, MinRotationPitch, MaxRotationPitch);

		if (ResultRotator.Pitch > PivotRotationPitch)
		{
			ResultRotator.Pitch = UKismetMathLibrary::MapRangeClamped
				(ResultRotator.Pitch, PivotRotationPitch, MaxRotationPitch, 0, MaxRotationPitch);
		}
		else
		{
			ResultRotator.Pitch = UKismetMathLibrary::MapRangeClamped
			(ResultRotator.Pitch, MinRotationPitch, PivotRotationPitch, MinRotationPitch, 0);
		}

		SetActorRotation(ResultRotator);
	}

	//

	if (CameraFocusStaticMeshComponent && TargetActor)
	{
		CameraFocusStaticMeshComponent->bHiddenInGame = false;
		CameraFocusStaticMeshComponent->SetWorldLocation(Cast<IUSATargetableInterface>(TargetActor)->GetTargetableToplocation());
	}
	

	float DesiredFOV = UKismetMathLibrary::MapRangeClamped
	(BetweenDistance, MinDistanceForFOV, MaxDistanceForFOV, MinFOVOffset, MaxFOVOffset);

	CameraComponent->FieldOfView = DesiredFOV;
}

