// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponBase.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Net/UnrealNetwork.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"


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
	WeaponMeshHolder = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Mesh Holder"));
	RootComponent = WeaponMeshHolder;

	// 메쉬 생성
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	WeaponMesh->SetupAttachment (WeaponMeshHolder);

	WeaponStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	WeaponStaticMesh->SetupAttachment(WeaponMeshHolder);

	WeaponBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Weapon Box Component"));
	WeaponBoxComponent->SetupAttachment(WeaponMeshHolder);
	
}

// Called when the game starts or when spawned
void AUSAWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUSAWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void AUSAWeaponBase::OnRep_WeaponOwner(AUSACharacterBase* PrevCharacter)
{
	//

	//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner"));


	if (WeaponOwner != nullptr)
	{
		//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner: Give"));

		GiveGameplayWeaponAbilitesToASC(WeaponOwner);
	}
	else
	{
		if (PrevCharacter != nullptr)
		{
			//USA_LOG(LogTemp, Log, TEXT("OnRep Weapon Owner: Clear"));

			ClearGameplayWeaponAbilitesToASC(PrevCharacter);
		}
	}

	//

	if (WeaponOwner == nullptr)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentTransformRules);

		SetActorRotation(FRotator::ZeroRotator);
		SetActorScale3D(FVector::OneVector);
	}
	else
	{
		WeaponOwner->AttachWeaponToHolderSocket(this);
	}
}

void AUSAWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AUSAWeaponBase, WeaponOwnerASC);
	DOREPLIFETIME(AUSAWeaponBase, WeaponOwner);
}

