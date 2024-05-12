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

#include "Kismet/KismetSystemLibrary.h"

#include "ProjectUSA.h"

// Sets default values
AUSAWeaponBase::AUSAWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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
	
	if (WeaponBoxComponent != nullptr)
	{
		WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSAWeaponBase::OnWeaponOverlapBegin);
	}
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

	WeaponOwnerASC = InASC;
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

		FGameplayAbilitySpec* GameplayAbilitySpec = InASC->FindAbilitySpecFromClass(Ability);

		if (GameplayAbilitySpec != nullptr)
		{
			InASC->ClearAbility(GameplayAbilitySpec->Handle);
		}

		//USA_LOG(LogTemp, Log, TEXT("Clear Ability"));
	}

	WeaponOwnerASC = nullptr;
}

void AUSAWeaponBase::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	if (WeaponOwnerASC != nullptr)
	{
		return;
	}

	IAbilitySystemInterface* InASCInterface = Cast <IAbilitySystemInterface>(OtherActor);
	UAbilitySystemComponent* InASC = nullptr;
	AUSACharacterBase* USACharacter = nullptr;

	if (InASCInterface != nullptr)
	{
		InASC = InASCInterface->GetAbilitySystemComponent();
	}

	if (InASC != nullptr)
	{
		USACharacter = Cast <AUSACharacterBase>(InASC->GetAvatarActor());
	}

	if (USACharacter != nullptr)
	{
		USACharacter->AddNextWaitingWeapon(this);
	}
}

void AUSAWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUSAWeaponBase, WeaponOwnerASC);
}

