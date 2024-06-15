// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/USATooltipSign.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
AUSATooltipSign::AUSATooltipSign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TooltipSignHolderComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Tooltip Holder Component"));
	RootComponent = TooltipSignHolderComponent;

	TooltipSignStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tooltip Static Mesh Component"));
	TooltipSignStaticMeshComponent->SetupAttachment(RootComponent);
	
	TooltipSignWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tooltip Sign Widget Component"));
	TooltipSignWidgetComponent->SetupAttachment(TooltipSignStaticMeshComponent);
}

// Called when the game starts or when spawned
void AUSATooltipSign::BeginPlay()
{
	Super::BeginPlay();

	K2_BeginPlay();
}

// Called every frame
void AUSATooltipSign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

