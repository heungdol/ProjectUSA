// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/USAItemSpawner.h"

#include "Item/USAItemBase.h"

#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AUSAItemSpawner::AUSAItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSpawnArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Item Spawn Arrow Component"));
	RootComponent = ItemSpawnArrowComponent;

	ItemSpawnStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Static Mesh Component"));
	ItemSpawnStaticMeshComponent->SetupAttachment(RootComponent);
	ItemSpawnStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	ItemSpawnStaticMeshComponent->SetHiddenInGame(true);
	ItemSpawnStaticMeshComponent->SetGenerateOverlapEvents(false);

	ItemSpawnTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Item Text Component"));
	ItemSpawnTextComponent->SetupAttachment(RootComponent);
	ItemSpawnTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	ItemSpawnTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	ItemSpawnTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	ItemSpawnTextComponent->SetHiddenInGame(true);
}

void AUSAItemSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemSpawnClass && Cast<AUSAItemBase>(ItemSpawnClass->GetDefaultObject()))
	{
		if (ItemSpawnStaticMeshComponent)
		{
			ItemSpawnStaticMeshComponent->SetStaticMesh(Cast<AUSAItemBase>(ItemSpawnClass->GetDefaultObject())->ItemDisplayStaticMeshRef);
		}

		if (ItemSpawnTextComponent)
		{
			ItemSpawnTextComponent->SetText(FText::FromString(TEXT("Item Spawn")));
		}
	}
}


// Called when the game starts or when spawned
void AUSAItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		GetWorld()->SpawnActor<AUSAItemBase>(ItemSpawnClass, GetActorLocation(), GetActorRotation());
	}
}

