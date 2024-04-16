// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/USAWeaponBase.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AUSAWeaponBase::AUSAWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 일단 메쉬를 루트 컴포넌트로 설정
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

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

//void AUSAWeaponBase::SetAbilitySystemComponent(UAbilitySystemComponent* InASC)
//{
//	ASC = InASC;
//}

//void AUSAWeaponBase::EquipWeapon()
//{
//	GiveGameplayWeaponAbilites();
//}
//
//void AUSAWeaponBase::UnequipWeapon()
//{
//	ClearGameplayWeaponAbilites();
//}

void AUSAWeaponBase::GiveGameplayWeaponAbilitesToASC(UAbilitySystemComponent* InASC)
{
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
		InASC->GiveAbility(GameplayAbilitySpec);
	}
}

void AUSAWeaponBase::ClearGameplayWeaponAbilitesToASC(UAbilitySystemComponent* InASC)
{
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