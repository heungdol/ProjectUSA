// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/USAWaypoint.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Character/USACharacterPlayer.h"
#include "Player/USAPlayerController.h"


#include "DrawDebugHelpers.h"


// Sets default values
AUSAWaypoint::AUSAWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaypointBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Waypoint Box Component"));
	RootComponent = WaypointBoxComponent;
	WaypointBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSAWaypoint::OnPlayerDetectBoxOverlapBegin);

	WaypointWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Waypoint Widget Component"));
	WaypointWidgetComponent->SetupAttachment(RootComponent);
	//WaypointWidgetComponent->SetVisibility(false);
	WaypointWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void AUSAWaypoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetWorld()->GetTimerManager().ClearTimer(DebugLineTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(DebugLineTimerHandle, this, &AUSAWaypoint::DrawLineToNextWaypoint, 1.0f, true);
}

void AUSAWaypoint::OnPlayerDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUSACharacterPlayer* USACharacterPlayer = Cast<AUSACharacterPlayer>(OtherActor);

	if (IsValid(USACharacterPlayer) == false)
	{
		return;
	}

	AUSAPlayerController* USAPlayerController = USACharacterPlayer->GetController<AUSAPlayerController>();

	if (IsValid(USAPlayerController) == false)
	{
		return;
	}

	if (USAPlayerController->IsLocalPlayerController() == false)
	{
		return;
	}

	if (bIsLastWaypoint)
	{
		return;
	}

	TArray<TObjectPtr<AActor>> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUSAWaypoint::StaticClass(), TempActors);

	for (AActor* TempActor : TempActors)
	{
		if (IsValid(TempActor) == false)
		{
			continue;
		}

		AUSAWaypoint* Waypoint = Cast<AUSAWaypoint>(TempActor);

		if (IsValid(Waypoint) == false)
		{
			return;
		}

		if (Waypoint == NextUSAWaypoint)
		{
			Waypoint->SetActiveWaypoint(true);
		}
		else
		{
			Waypoint->SetActiveWaypoint(false);
		}
	}
}

void AUSAWaypoint::SetActiveWaypoint(bool InActive)
{
	WaypointWidgetComponent->SetVisibility(InActive);

	K2_SetActiveWaypoint(InActive);
}

// Called when the game starts or when spawned
void AUSAWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (bIsFirstWaypoint)
	{
		SetActiveWaypoint(true);
	}
	else
	{
		SetActiveWaypoint(false);
	}
}

void AUSAWaypoint::DrawLineToNextWaypoint()
{
	if (IsValid(NextUSAWaypoint))
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), NextUSAWaypoint->GetActorLocation(), FColor::Green, false, 1.0f, 0U, 10.f);
	}
}

// Called every frame
void AUSAWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

