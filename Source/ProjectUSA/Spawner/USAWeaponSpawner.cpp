// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner/USAWeaponSpawner.h"

#include "Weapon/USAWeaponBase.h"

#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


AUSAWeaponSpawner::AUSAWeaponSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponSpawnArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Weapon Spawn Arrow Component"));
	RootComponent = WeaponSpawnArrowComponent;

	WeaponSpawnStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Static Mesh Component"));
	WeaponSpawnStaticMeshComponent->SetupAttachment(RootComponent);
	WeaponSpawnStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponSpawnStaticMeshComponent->SetHiddenInGame(true);
	WeaponSpawnStaticMeshComponent->SetGenerateOverlapEvents(false);

	WeaponSpawnSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh Component"));
	WeaponSpawnSkeletalMeshComponent->SetupAttachment(RootComponent);
	WeaponSpawnSkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponSpawnSkeletalMeshComponent->SetHiddenInGame(true);
	WeaponSpawnSkeletalMeshComponent->SetGenerateOverlapEvents(false);

	WeaponSpawnTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Weapon Text Component"));
	WeaponSpawnTextComponent->SetupAttachment(RootComponent);
	WeaponSpawnTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	WeaponSpawnTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	WeaponSpawnTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	WeaponSpawnTextComponent->SetHiddenInGame(true);
}

void AUSAWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (WeaponSpawnClass && Cast<AUSAWeaponBase>(WeaponSpawnClass->GetDefaultObject()))
	{
		WeaponSpawnStaticMeshComponent->SetStaticMesh(Cast<AUSAWeaponBase>(WeaponSpawnClass->GetDefaultObject())->WeaponDisplayStaticMeshRef);
		WeaponSpawnSkeletalMeshComponent->SetSkeletalMesh(Cast<AUSAWeaponBase>(WeaponSpawnClass->GetDefaultObject())->WeaponDisplaySkeletalMeshMeshRef);

		FText SpawnWeaponText = FText::Format(NSLOCTEXT("", "", "Spawn Weapon: {0}"), WeaponSpawnClass->GetDisplayNameText());
		WeaponSpawnTextComponent->SetText(SpawnWeaponText);
	}
}

void AUSAWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		GetWorld()->SpawnActor<AUSAWeaponBase>(WeaponSpawnClass, GetActorLocation(), GetActorRotation());
	}
}
