// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponBase.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Net/UnrealNetwork.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "GameFramework/RotatingMovementComponent.h"


#include "Kismet/KismetSystemLibrary.h"

#include "ProjectUSA.h"

// Sets default values
AUSAWeaponBase::AUSAWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// 메쉬 홀더를 루트로 설정 -> 메쉬의 중심을 바꾸기 위함
	WeaponCollisionComponent = CreateDefaultSubobject <USphereComponent>(TEXT("Weapon Mesh Holder"));
	RootComponent = WeaponCollisionComponent;
	WeaponCollisionComponent->SetSimulatePhysics(true);
	WeaponCollisionComponent->SetCollisionProfileName(TEXT("Item"), false);
	WeaponCollisionComponent->SetGenerateOverlapEvents(false);
	WeaponCollisionComponent->SetNotifyRigidBodyCollision(true);
	WeaponCollisionComponent->OnComponentHit.AddDynamic(this, &AUSAWeaponBase::OnBoxComponentHitAndCheckIsGround);

	WeaponCollisionComponent->BodyInstance.bLockXRotation = true;
	WeaponCollisionComponent->BodyInstance.bLockYRotation = true;
	//WeaponCollisionComponent->BodyInstance.bLockZRotation = true;

	// 메쉬 생성
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	WeaponMesh->SetupAttachment (WeaponCollisionComponent);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
	WeaponMesh->SetGenerateOverlapEvents(false);

	WeaponStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	WeaponStaticMesh->SetupAttachment(WeaponCollisionComponent);
	WeaponStaticMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
	WeaponStaticMesh->SetGenerateOverlapEvents(false);

	// 범위 설정
	WeaponBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Weapon Box Component"));
	WeaponBoxComponent->SetupAttachment(WeaponCollisionComponent);
	WeaponBoxComponent->SetCollisionProfileName(TEXT("OverlapAll"), false);
	WeaponBoxComponent->SetSimulatePhysics(false);
	WeaponBoxComponent->SetGenerateOverlapEvents(true);

	// 회전 설정
	//WeaponRotatingMovementCompont = CreateDefaultSubobject <URotatingMovementComponent>(TEXT("Weapon Rotating Movement Component"));
	//WeaponRotatingMovementCompont->SetAutoActivate(false);
}

void AUSAWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponMesh->SetSkeletalMesh(WeaponMeshRef);
	WeaponStaticMesh->SetStaticMesh(WeaponStaticMeshRef);
}

// Called when the game starts or when spawned
void AUSAWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//if (IsValid(WeaponOwner) == true)
	//{
	//	if (IsValid(WeaponRotatingMovementCompont))
	//	{
	//		WeaponRotatingMovementCompont->SetActive(false);
	//	}
	//}
	//else
	//{
	//	if (IsValid(WeaponRotatingMovementCompont))
	//	{
	//		WeaponRotatingMovementCompont->SetActive(true);
	//	}
	//}
}

// Called every frame
void AUSAWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsWeaponDropping == false
	//	&& IsValid(WeaponOwner) == false)
	//{
	//	//WeaponMesh->SetComponentRotate
	//}
}

void AUSAWeaponBase::SetWeaponOwner(AUSACharacterBase* InCharacter)
{
	if (UKismetSystemLibrary::IsServer (GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone (GetWorld()) == false)
	{
		return;
	}

	AUSACharacterBase* PrevCharacter = WeaponOwner;
	WeaponOwner = InCharacter;

	OnRep_WeaponOwner(PrevCharacter);
}

void AUSAWeaponBase::SetWeaponPhysics(bool IsDropping)
{
	if (IsDropping == true)
	{
		if (IsValid(WeaponCollisionComponent) == true)
		{
			// 기본 설정
			WeaponCollisionComponent->SetSimulatePhysics(true);

			WeaponCollisionComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

			// 랜덤 값은 서버에서만 수행
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
				WeaponCollisionComponent->SetPhysicsLinearVelocity(WeaponImpulseVector);
			}
		}
	}
	else
	{
		if (IsValid(WeaponCollisionComponent) == true)
		{
			WeaponCollisionComponent->SetSimulatePhysics(false);
		}

		//if (IsValid(WeaponRotatingMovementCompont))
		//{
		//	WeaponRotatingMovementCompont->SetActive(false);
		//}
	}

}



void AUSAWeaponBase::GiveGameplayWeaponAbilitesToASC(AUSACharacterBase* InCharacter)
{
	IAbilitySystemInterface* ASCInterface = Cast <IAbilitySystemInterface>(InCharacter);

	if (ASCInterface == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* InASC = ASCInterface->GetAbilitySystemComponent();

	if (InASC == nullptr)
	{
		return;
	}

	if (InCharacter == nullptr)
	{
		return;
	}

	if (HasAuthority() == true)
	{
		for (const auto& Ability : GameplayWeaponAbilities)
		{
			if (Ability == nullptr)
			{
				continue;
			}

			if (IsValid(Ability) == false)
			{
				continue;
			}

			FGameplayAbilitySpec GameplayAbilitySpec(Ability);
			InASC->GiveAbility(GameplayAbilitySpec);
		}
	}

	InCharacter->SetCurrentWeapon(WeaponType, this);
	InCharacter->K2_OnUSACurrentWeaponChanged(WeaponType, this);
}

void AUSAWeaponBase::ClearGameplayWeaponAbilitesToASC(AUSACharacterBase* InCharacter)
{
	IAbilitySystemInterface* ASCInterface = Cast <IAbilitySystemInterface>(InCharacter);

	if (ASCInterface == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* InASC = ASCInterface->GetAbilitySystemComponent();

	if (InASC == nullptr)
	{
		return;
	}

	if (InCharacter == nullptr)
	{
		return;
	}

	if (HasAuthority() == true)
	{
		for (const auto& Ability : GameplayWeaponAbilities)
		{

			if (Ability == nullptr)
			{
				continue;
			}

			if (IsValid(Ability) == false)
			{
				continue;
			}

			FGameplayAbilitySpec* GameplayAbilitySpec = InASC->FindAbilitySpecFromClass(Ability);

			if (GameplayAbilitySpec != nullptr)
			{
				InASC->ClearAbility(GameplayAbilitySpec->Handle);
			}
		}
	}

	InCharacter->SetCurrentWeapon(WeaponType, nullptr);
	InCharacter->K2_OnUSACurrentWeaponChanged(WeaponType, nullptr);
}

void AUSAWeaponBase::OnBoxComponentHitAndCheckIsGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 서버에서만 수행
	//if (UKismetSystemLibrary::IsServer(GetWorld()) == false)
	//{
	//	return;
	//}

	if (IsValid(WeaponOwner) == true)
	{
		return;
	}

	const float DotThreshold = 0.45f;
	if (FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector) > DotThreshold)
	{
		if (IsValid(WeaponCollisionComponent) == true)
		{
			WeaponCollisionComponent->SetSimulatePhysics(false);
			WeaponCollisionComponent->SetSimulatePhysics(true);
		}

		//if (IsValid(WeaponRotatingMovementCompont) == true)
		//{
		//	WeaponRotatingMovementCompont->SetActive(true);
		//}
	}
}

void AUSAWeaponBase::OnRep_WeaponOwner(AUSACharacterBase* PrevCharacter)
{
	//

	//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner"));


	if (IsValid(WeaponOwner) == true)
	{
	//	//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner: Give"));

		GiveGameplayWeaponAbilitesToASC(WeaponOwner);
	}
	else
	{
		if (IsValid(PrevCharacter) == true)
		{
			//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner: Clear"));

			ClearGameplayWeaponAbilitesToASC(PrevCharacter);
		}
	}

	////

	if (WeaponOwner == nullptr
		|| IsValid(WeaponOwner) == false)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentTransformRules);

		if (PrevCharacter != nullptr)
		{
			SetActorLocation(PrevCharacter->GetActorLocation());
		}

		SetActorRotation(FRotator::ZeroRotator);
		SetActorScale3D(FVector::OneVector);

		SetWeaponPhysics(true);
	}
	else
	{
		SetWeaponPhysics(false);
		WeaponOwner->AttachWeaponToHolderSocket(this);
	}
}

void AUSAWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AUSAWeaponBase, WeaponOwnerASC);
	DOREPLIFETIME(AUSAWeaponBase, WeaponOwner);
}

