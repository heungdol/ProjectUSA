// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponBase.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"


#include "ProjectUSA.h"

// Sets default values
AUSAWeaponBase::AUSAWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 홀더를 루트로 설정 -> 메쉬의 중심을 바꾸기 위함
	WeaponMeshHolder = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Mesh Holder"));
	RootComponent = WeaponMeshHolder;

	// 메쉬 생성
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	WeaponMesh->SetupAttachment (WeaponMeshHolder);

	WeaponStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	WeaponStaticMesh->SetupAttachment(WeaponMeshHolder);
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

void AUSAWeaponBase::GiveGameplayWeaponAbilitesToASC(UAbilitySystemComponent* InASC)
{
	if (InASC == nullptr)
	{
		USA_LOG(LogTemp, Log, TEXT("No ASC"));
		return;
	}

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

void AUSAWeaponBase::ClearGameplayWeaponAbilitesToASC(UAbilitySystemComponent* InASC)
{
	if (HasAuthority() == false)
	{
		return;
	}

	if (InASC == nullptr)
	{
		return;
	}

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
		InASC->ClearAbility(GameplayAbilitySpec.Handle);
	}
}