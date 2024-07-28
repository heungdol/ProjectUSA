// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/USATargetingCameraActor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Data/USAJellyEffectData.h"
#include "Component/USAJellyEffectComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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

	CameraFocusStaticMeshHolder = CreateDefaultSubobject <USceneComponent>(TEXT("Camera Target Static Mesh Parent"));

	CameraFocusStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Camera Target Static Mesh Component"));
	CameraFocusStaticMeshComponent->SetupAttachment(CameraFocusStaticMeshHolder);
	CameraFocusStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	CameraFocusStaticMeshComponent->SetGenerateOverlapEvents(false);

	CameraFocusJelleyEffectComponent = CreateDefaultSubobject <UUSAJellyEffectComponent>(TEXT("Jelly Effect Component"));
}

// Called when the game starts or when spawned
void AUSATargetingCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (CameraFocusStaticMeshComponent)
	{
		CameraFocusStaticMeshComponent->SetVisibility(false);
	}

	if (CameraFocusJelleyEffectComponent && CameraFocusStaticMeshComponent)
	{
		CameraFocusJelleyEffectComponent->SetJellySceneComponent(nullptr, CameraFocusStaticMeshComponent);
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

void AUSATargetingCameraActor::SetSourceActor(AActor* const InActor)
{
	SourceActor = InActor;
}

void AUSATargetingCameraActor::SetTargetActor(AActor* const InActor)
{
	TargetActor = InActor;

	if (CameraFocusStaticMeshComponent && CameraFocusJelleyEffectComponent)
	{
		if (InActor)
		{
			if (!bIsOn)
			{
				CameraFocusStaticMeshComponent->SetVisibility(true);
				CameraFocusJelleyEffectComponent->PlayJellyEffect(FocusShowJellyEffectData);

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), CameraFocusSoundOn, GetActorLocation());
			}

			bIsOn = true;
		}
		else
		{
			if (bIsOn)
			{
				CameraFocusJelleyEffectComponent->PlayJellyEffect(FocusHideJellyEffectData);

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), CameraFocusSoundOff, GetActorLocation());
			}
		
			bIsOn = false;
		}
	}
}

void AUSATargetingCameraActor::CalculateTargetingCameraTransform()
{
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

	const FVector& CameraLocation = CameraComponent->GetComponentLocation();

	const FVector& SourceLocation 
		= Cast<IUSATargetableInterface>(SourceActor)
		->GetTargetableLocationByPivotType(ETargetablePivotType::Center);

	const FVector& TargetLocation 
		= Cast<IUSATargetableInterface>(TargetActor)
		->GetTargetableLocationByPivotType(ETargetablePivotType::Center);

	//

	FVector ResultLocation = ((SourceLocation * LocationWeightRatio) + (TargetLocation * (1.0f - LocationWeightRatio)));
	ResultLocation.Z += OffsetResultLocationHeight;

	FVector ResultDirection = TargetLocation - SourceLocation;
	ResultDirection.Normalize();

	SetActorLocation(ResultLocation);
	
	float BetweenDistance = (SourceLocation - TargetLocation).Length();

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

	//

	if (CameraFocusStaticMeshHolder && TargetActor)
	{
		CameraFocusStaticMeshHolder->SetWorldLocation
			(
				Cast<IUSATargetableInterface>(TargetActor)
				->GetTargetableLocationByPivotType(ETargetablePivotType::Top)
			);
	}

	// DesiredFOV
	CameraComponent->FieldOfView = UKismetMathLibrary::MapRangeClamped
	(BetweenDistance, MinDistanceForFOV, MaxDistanceForFOV, MinFOVOffset, MaxFOVOffset);
}

