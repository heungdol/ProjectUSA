// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/USAPlacedCameraActor.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "Camera/CameraComponent.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

// Sets default values
AUSAPlacedCameraActor::AUSAPlacedCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	TotalHolderComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Placed Camera Holder Component"));
	RootComponent = TotalHolderComponent;

	InBoxHolderComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Placed Camera In Box Holder Component"));
	InBoxHolderComponent->SetupAttachment(RootComponent);

	OutBoxHolderComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Placed Camera Out Box Holder Component"));
	OutBoxHolderComponent->SetupAttachment(RootComponent);

	DirectionArrowComponent = CreateDefaultSubobject <UArrowComponent>(TEXT("Placed Camera Arrow Component"));
	DirectionArrowComponent->SetupAttachment(RootComponent);

	//InOverlapBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Placed Camera In Box Component"));
	//InOverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSAPlacedCameraActor::OnBoxOverlapBegin);
	//InOverlapBoxComponent->SetGenerateOverlapEvents(true);
	//InOverlapBoxComponent->SetupAttachment(RootComponent);

	//OutOverlapBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Placed Camera Out Box Component"));
	//OutOverlapBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUSAPlacedCameraActor::OnBoxOverlapEnd);
	//OutOverlapBoxComponent->SetGenerateOverlapEvents(true);
	//OutOverlapBoxComponent->SetupAttachment(RootComponent);

	FrontCameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("Placed Camera Front Camera Component"));
	FrontCameraComponent->SetupAttachment(RootComponent);

	BackCameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("Placed Camera Back Camera Component"));
	BackCameraComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AUSAPlacedCameraActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InBoxHolderComponent) == true)
	{
		for (USceneComponent* InBox : InBoxHolderComponent->GetAttachChildren())
		{
			if (IsValid(InBox) == false)
			{
				continue;
			}
			
			UBoxComponent* BoxComponent = Cast <UBoxComponent>(InBox);

			if (IsValid(BoxComponent) == false)
			{
				continue;
			}

			BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSAPlacedCameraActor::OnBoxOverlapBegin);
			BoxComponent->SetGenerateOverlapEvents(true);
		}
	}

	if (IsValid(OutBoxHolderComponent) == true)
	{
		for (USceneComponent* OutBox : OutBoxHolderComponent->GetAttachChildren())
		{
			if (IsValid(OutBox) == false)
			{
				continue;
			}

			UBoxComponent* BoxComponent = Cast <UBoxComponent>(OutBox);

			if (IsValid(BoxComponent) == false)
			{
				continue;
			}

			BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUSAPlacedCameraActor::OnBoxOverlapEnd);
			BoxComponent->SetGenerateOverlapEvents(true);
		}
	}
}

// Called every frame
void AUSAPlacedCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSAPlacedCameraActor::InitPlacedCameraActor(const FVector& InActorForward, const FVector& InCameraForward)
{
	FVector ArrowForward = FVector::ForwardVector;

	if (IsValid(DirectionArrowComponent) == true)
	{
		ArrowForward = DirectionArrowComponent->GetComponentRotation().Vector();
	}

	float DotScore = 0.0f;

	if (bIsBasedCameraRotation)
	{
		DotScore = FVector::DotProduct(InCameraForward, ArrowForward);
	}
	else
	{
		DotScore = FVector::DotProduct(InActorForward, ArrowForward);
	}

	if (DotScore > 0)
	{
		if (IsValid(FrontCameraComponent) == true)
		{
			FrontCameraComponent->SetActive(true);
		}

		if (IsValid(BackCameraComponent) == true)
		{
			BackCameraComponent->SetActive(false);
		}
	}
	else
	{
		if (IsValid(FrontCameraComponent) == true)
		{
			FrontCameraComponent->SetActive(false);
		}

		if (IsValid(BackCameraComponent) == true)
		{
			BackCameraComponent->SetActive(true);
		}
	}

}

FRotator AUSAPlacedCameraActor::GetActiveCameraRotation()
{
	FRotator Result = FVector::ForwardVector.Rotation();

	if (IsValid(FrontCameraComponent) == true
		&& FrontCameraComponent->IsActive() == true)
	{
		Result = FrontCameraComponent->GetComponentRotation();
	}

	if (IsValid(BackCameraComponent) == true
		&& BackCameraComponent->IsActive() == true)
	{
		Result = BackCameraComponent->GetComponentRotation();
	}

	return Result;
}

void AUSAPlacedCameraActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUSACharacterPlayer* PlayerActor = Cast <AUSACharacterPlayer>(OtherActor))
	{
		PlayerActor->StartPlacedCamera(this);

		TargetActor = OtherActor;
	}
}

void AUSAPlacedCameraActor::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AUSACharacterPlayer* PlayerActor = Cast <AUSACharacterPlayer>(OtherActor))
	{
		if (TargetActor == OtherActor)
		{
			PlayerActor->FinishPlacedCamera(this);

			TargetActor = nullptr;
		}
	}
}