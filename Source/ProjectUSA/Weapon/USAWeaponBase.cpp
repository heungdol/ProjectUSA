// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponBase.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Components/MeshComponent.h"
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
	//WeaponCollisionComponent = CreateDefaultSubobject <USphereComponent>(TEXT("Weapon Mesh Holder"));
	//RootComponent = WeaponCollisionComponent;
	//WeaponCollisionComponent->SetSimulatePhysics(true);
	//WeaponCollisionComponent->SetCollisionProfileName(TEXT("Item"), false);
	//WeaponCollisionComponent->SetGenerateOverlapEvents(false);
	//WeaponCollisionComponent->SetNotifyRigidBodyCollision(true);
	//WeaponCollisionComponent->OnComponentHit.AddDynamic(this, &AUSAWeaponBase::OnBoxComponentHitAndCheckIsGround);

	//WeaponCollisionComponent->BodyInstance.bLockXRotation = true;
	//WeaponCollisionComponent->BodyInstance.bLockYRotation = true;
	//WeaponCollisionComponent->BodyInstance.bLockZRotation = true;

	// 메쉬 생성
	//WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	//WeaponMesh->SetupAttachment (WeaponCollisionComponent);
	//WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
	//WeaponMesh->SetGenerateOverlapEvents(false);

	//WeaponStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	//WeaponStaticMesh->SetupAttachment(WeaponCollisionComponent);
	//WeaponStaticMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
	//WeaponStaticMesh->SetGenerateOverlapEvents(false);

	//// 범위 설정
	//WeaponBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Weapon Box Component"));
	//WeaponBoxComponent->SetupAttachment(WeaponCollisionComponent);
	//WeaponBoxComponent->SetCollisionProfileName(TEXT("OverlapAll"), false);
	//WeaponBoxComponent->SetSimulatePhysics(false);
	//WeaponBoxComponent->SetGenerateOverlapEvents(true);

	// 회전 설정
	//WeaponRotatingMovementCompont = CreateDefaultSubobject <URotatingMovementComponent>(TEXT("Weapon Rotating Movement Component"));
	//WeaponRotatingMovementCompont->SetAutoActivate(false);

	//WeaponMeshComponent = CreateDefaultSubobject <UMeshComponent>(TEXT("Weapon Skeletal Mesh"));
}

void AUSAWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//WeaponMesh->SetSkeletalMesh(WeaponMeshRef);
	//WeaponStaticMesh->SetStaticMesh(WeaponStaticMeshRef);
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

void AUSAWeaponBase::SetWeaponPhysics(bool IsDropping, bool IsFirst)
{


}

void AUSAWeaponBase::PickUpByUSACharacter(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	if (IsValid(InASC) == false)
	{
		return;
	}

	if (IsValid(InCharacter) == false)
	{
		return;
	}

	if (GetWeaponOwner() != nullptr)
	{
		return;
	}

	if ((uint8)WeaponType < 0 || InCharacter->CurrentEquipedWeapons.Num() <= (uint8)WeaponType)
	{
		return;
	}

	if (InCharacter->CurrentEquipedWeapons[(uint8)WeaponType] != nullptr)
	{
		return;
	}

	// ASC 새 갱신
	SetWeaponOwner(InCharacter);
}

//void AUSAWeaponBase::DropDownFromUSACharacter(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
//{
//
//}


bool AUSAWeaponBase::GiveGameplayWeaponAbilitesToASC(AUSACharacterBase* InCharacter)
{
	IAbilitySystemInterface* ASCInterface = Cast <IAbilitySystemInterface>(InCharacter);

	if (ASCInterface == nullptr)
	{
		return false;
	}

	UAbilitySystemComponent* InASC = ASCInterface->GetAbilitySystemComponent();

	if (IsValid(InASC) == false)
	{
		return false;
	}

	if (IsValid(InCharacter) == false)
	{
		return false;
	}

	if (InCharacter->SetCurrentWeapon(WeaponType, this) == false)
	{
		return false;
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

	return true;
}

bool AUSAWeaponBase::ClearGameplayWeaponAbilitesToASC(AUSACharacterBase* InCharacter)
{
	IAbilitySystemInterface* ASCInterface = Cast <IAbilitySystemInterface>(InCharacter);

	if (ASCInterface == nullptr)
	{
		return false;
	}

	UAbilitySystemComponent* InASC = ASCInterface->GetAbilitySystemComponent();

	if (InASC == nullptr)
	{
		return false;
	}

	if (InCharacter == nullptr)
	{
		return false;
	}

	if (InCharacter->SetCurrentWeapon(WeaponType, nullptr) == false)
	{
		return false;
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

	return true;
}

//void AUSAWeaponBase::OnBoxComponentHitAndCheckIsGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	// 서버에서만 수행
//	//if (UKismetSystemLibrary::IsServer(GetWorld()) == false)
//	//{
//	//	return;
//	//}
//
//	if (IsValid(WeaponOwner) == true)
//	{
//		return;
//	}
//
//	const float DotThreshold = 0.45f;
//	if (FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector) > DotThreshold)
//	{
//		if (IsValid(WeaponCollisionComponent) == true)
//		{
//			WeaponCollisionComponent->SetSimulatePhysics(false);
//			WeaponCollisionComponent->SetSimulatePhysics(true);
//		}
//
//		//if (IsValid(WeaponRotatingMovementCompont) == true)
//		//{
//		//	WeaponRotatingMovementCompont->SetActive(true);
//		//}
//	}
//}

void AUSAWeaponBase::OnRep_WeaponOwner(AUSACharacterBase* PrevCharacter)
{
	if (IsValid(PrevCharacter) == true)
	{
		ClearGameplayWeaponAbilitesToASC(PrevCharacter);

		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentTransformRules);
		
		PrevCharacter->K2_OnUSACurrentWeaponChanged(WeaponType, nullptr);

		SetWeaponPhysics(true);
	}

	if (IsValid(WeaponOwner) == true)
	{
		if (GiveGameplayWeaponAbilitesToASC(WeaponOwner))
		{
			WeaponOwner->K2_OnUSACurrentWeaponChanged(WeaponType, this);

			UAnimInstance* AnimInstance = (WeaponOwner->GetMesh()) ? WeaponOwner->GetMesh()->GetAnimInstance() : nullptr;

			if (IsValid(WeaponPickUpAnimMontage) == true && IsValid(AnimInstance) == true)
			{
				WeaponOwner->AttachWeaponToHandSocket(this);
				AnimInstance->Montage_Play(WeaponPickUpAnimMontage, WeaponPickUpAnimMontageRate, EMontagePlayReturnType::MontageLength, 0.0f, false);
			}
			else
			{
				WeaponOwner->AttachWeaponToHolderSocket(this);
			}

			SetWeaponPhysics(false);
		}
	}
}

void AUSAWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AUSAWeaponBase, WeaponOwnerASC);
	DOREPLIFETIME(AUSAWeaponBase, WeaponOwner);
}

