// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/USAAttackActor.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "GameFramework/Character.h"

#include "Character/USACharacterBase.h"
//#include "Character/USACharacterNonPlayer.h"
//#include "Character/USACharacterPlayer.h"

#include "Interface/USADamageableInterface.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/StaticMeshActor.h"
#include "Engine/Brush.h"


// Sets default values
AUSAAttackActor::AUSAAttackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;


	AttackProjectileMovementComponent = CreateDefaultSubobject <UProjectileMovementComponent>(TEXT("Attack Projectile Movement Component"));

	AttackSphereComponent = CreateDefaultSubobject <USphereComponent>(TEXT("Attack Sphere Component"));
	RootComponent = AttackSphereComponent;
	AttackSphereComponent->SetGenerateOverlapEvents(true);
	//AttackSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSAAttackActor::OnOverlapTargetActor);

	AttackStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Attack Static Mesh Component"));
	AttackStaticMeshComponent->SetupAttachment(AttackSphereComponent);
	AttackStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	AttackStaticMeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void AUSAAttackActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetAuthGameMode())
	{
		AttackStartTime = GetWorld()->GetTimeSeconds();
	}

	PrevLocation = GetActorLocation();

	if (IsValid(AttackSphereComponent) == true)
	{
		TraceRadius = AttackSphereComponent->GetScaledSphereRadius();
	}

	SetLifeSpan(AttackDurtaion);
}

void AUSAAttackActor::OnOverlapTargetActor
(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	if (bIsInitialized == false)
	{
		return;
	}

	if (IsValid(OtherActor) == true)
	{
		TryToGiveDamageToActor(OtherActor, SweepResult);
		TryToGiveDamageToActor(OtherActor->GetParentActor(), SweepResult);
		TryToGiveDamageToActor(OtherActor->GetAttachParentActor(), SweepResult);
	}
}

void AUSAAttackActor::TryToGiveDamageToActor(AActor* InActor, const FHitResult& SweepResult)
{
	if (AttackStartTime + AttackTraceDurtaion < GetWorld()->GetTimeSeconds())
	{
		return;
	}

	if (IsValid(InActor) == false)
	{
		return;
	}

	if (OutActors.Contains(InActor) == true)
	{
		return;
	}

	OutActors.Add(InActor);

	if (bIsAttackableBySelf == false
		&& (InActor == OriginActor))
	{
		return;
	}

	//

	IUSADamageableInterface* USADamageableInterface = Cast<IUSADamageableInterface>(InActor);
	if (USADamageableInterface != nullptr)
	{
		FVector AttackDirection = GetActorForwardVector();
		FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, SweepResult, AttackDirection, AttackDamageType);

		USADamageableInterface->ApplyDamageHitNiagaraEffect(OriginController, OriginActor, AttackHitNiagaraSystemObject,
			bIsAttackHitNiagaraOffset);

		USADamageableInterface->TakeDamage(AttackDamage, AttackDamageEvent, OriginController, this);

		bIsHittingActor = true;
	}
	else if (IsValid(Cast<AStaticMeshActor>(InActor)) == true)
	{
		bIsHittingStaticMeshActor = true;
	}
}

// Called every frame
void AUSAAttackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	if (bIsInitialized == false)
	{
		return;
	}

	{
		// Pawn
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti
		(GetWorld(),
			PrevLocation,
			CurrentLocation,
			TraceRadius, 
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
			false,
			OutActors,
			EDrawDebugTrace::None,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.1f);

		for (FHitResult HitResultOne : HitResults)
		{
			TryToGiveDamageToActor(HitResultOne.GetActor(), HitResultOne);
			
			if (bIsHittingActor == true
				&& bIsUsingSingleTrace == true)
			{
				Destroy();
				break;
			}

			if (bIsHittingStaticMeshActor == true
				&& bIsDetectingStaticMesh == true)
			{
				Destroy();
				break;
			}
		}
	}

	PrevLocation = CurrentLocation;
}

void AUSAAttackActor::BeginDestroy()
{
	Super::BeginDestroy();


}

void AUSAAttackActor::Destroyed()
{
	if (GetWorld()->WorldType == EWorldType::Editor)
	{
		return;
	}

	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	if (IsValid(NextSpawnAttackActorClass) == true)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(NextSpawnAttackActorClass, SpawnLocation, SpawnRotation);
		IUSASpawnableInterface* SpawnableInterface = Cast <IUSASpawnableInterface>(SpawnActor);

		if (SpawnableInterface != nullptr)
		{
			SpawnableInterface->InitUSASpawnableActor(OriginActor, OriginController);
		}
	
	}

	Super::Destroyed();
}

void AUSAAttackActor::InitUSASpawnableActor(AActor* InOriginActor, AController* InController)
{
	OriginActor = InOriginActor;
	OriginController = InController;

	bIsInitialized = true;
	bIsHittingActor = false;
	bIsHittingStaticMeshActor = false;

	OutActors.Init(this, 1);
	//OutActors.Add(OriginActor);
}

bool AUSAAttackActor::GetIsInitialized()
{
	return bIsInitialized;
}

