// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponStaticMesh.h"

#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Kismet/KismetSystemLibrary.h"


AUSAWeaponStaticMesh::AUSAWeaponStaticMesh()
{
	// 메쉬 생성
	WeaponMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	RootComponent = WeaponMeshComponent;

	WeaponMeshComponent->SetCollisionProfileName(TEXT("Item"), false);
	WeaponMeshComponent->SetGenerateOverlapEvents(/*true*/false);
	WeaponMeshComponent->SetSimulatePhysics(true);

	WeaponMeshComponent->BodyInstance.bOverrideMass = true;
	WeaponMeshComponent->BodyInstance.SetMassOverride (200.0f);
	//WeaponMeshComponent->SetMassOverrideInKg(NAME_None, 200.0f, true);
	WeaponMeshComponent->SetAngularDamping(1.0f);
	WeaponMeshComponent->SetNotifyRigidBodyCollision(false);

	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Component"));
	WeaponBoxComponent->SetupAttachment(RootComponent);
	WeaponBoxComponent->SetGenerateOverlapEvents(true);

}
 
void AUSAWeaponStaticMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//if (IsValid(WeaponMeshComponent) == true)
	//{
	//	WeaponMeshComponent->SetStaticMesh(WeaponStaticMeshRef);
	//}
}

void AUSAWeaponStaticMesh::BeginPlay()
{
	Super::BeginPlay();

	//SetWeaponPhysics(true, true);
}

void AUSAWeaponStaticMesh::SetWeaponPhysics(bool IsDropping, bool IsFirst)
{
	Super::SetWeaponPhysics(IsDropping);

	if (IsDropping == true)
	{
		if (IsValid(WeaponMeshComponent) == true)
		{
			WeaponMeshComponent->SetSimulatePhysics(true);
			WeaponMeshComponent->SetCollisionProfileName(TEXT("Item"), true);
			//WeaponMeshComponent->SetGenerateOverlapEvents(true);

			//WeaponMeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

			// 랜덤 값은 서버에서만 수행
			if (IsFirst == false)
			{
				if (UKismetSystemLibrary::IsServer(GetWorld()) == true)
				{
					FVector WeaponImpulseVector = FVector::UpVector * WeaponDropImpulseZ;

					if (bIsRandomDrop == false)
					{
						WeaponImpulseVector += GetActorForwardVector() * WeaponDropImpulseXY;
					}
					else
					{
						WeaponImpulseVector += FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Vector() * WeaponDropImpulseXY;
					}

					
					WeaponMeshComponent->SetPhysicsLinearVelocity(WeaponImpulseVector);
					
					//MulticastRPC_ImpulseWeapon(GetActorLocation(), GetActorRotation(), GetActorScale3D(), WeaponImpulseVector);
				}
			}
		}
	}
	else
	{
		if (IsValid(WeaponMeshComponent) == true)
		{
			WeaponMeshComponent->SetSimulatePhysics(false);
			WeaponMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"), true);
			//WeaponMeshComponent->SetGenerateOverlapEvents(false);
		}
	}
}

//void AUSAWeaponStaticMesh::MulticastRPC_ImpulseWeapon_Implementation(const FVector& InLocation, const FRotator& InRotatoin, const FVector& InScale, const FVector& InImpulse)
//{
//	SetActorLocation(InLocation);
//	SetActorRotation(InRotatoin);
//	SetActorScale3D(InScale);
//
//	if (IsValid(WeaponMeshComponent) == true)
//	{
//		WeaponMeshComponent->SetPhysicsLinearVelocity(InImpulse);
//	}
//}

