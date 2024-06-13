// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USAItemBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "Character/USACharacterBase.h"
//#include "Character/USACharacterPlayer.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Tag/USAGameplayTags.h"

#include "AbilitySystemComponent.h"

// Sets default values
AUSAItemBase::AUSAItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Static Mesh Component"));
	RootComponent = ItemStaticMeshComponent;

	ItemStaticMeshComponent->SetCollisionProfileName(TEXT("Item"), false);
	ItemStaticMeshComponent->SetGenerateOverlapEvents(/*true*/false);
	ItemStaticMeshComponent->SetSimulatePhysics(true);

	ItemStaticMeshComponent->BodyInstance.bOverrideMass = true;
	ItemStaticMeshComponent->BodyInstance.SetMassOverride(200.0f);
	ItemStaticMeshComponent->SetAngularDamping(1.0f);
	ItemStaticMeshComponent->SetNotifyRigidBodyCollision(false);

	ItemSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Item Sphere Component"));
	ItemSphereComponent->SetupAttachment(RootComponent);
	ItemSphereComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AUSAItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSAItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AUSAItemBase::PickUpByUSACharacter(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	if (IsValid(InCharacter) == false)
	{
		return false;
	}

	InCharacter->CurrentOwnedItems.Add(ItemSubclass);

	Destroy();

	return true;
}

void AUSAItemBase::PlayPickUpAnimationMontageInUSACharacter(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	if (IsValid(InCharacter) == false)
	{
		return;
	}

	UAnimInstance* AnimInstance = (InCharacter->GetMesh()) ? InCharacter->GetMesh()->GetAnimInstance() : nullptr;

	if (IsValid(ItemPickUpAnimMontage) == true
		&& IsValid(AnimInstance) == true
		&& IsValid(InASC) == true
		&& InASC->GetGameplayTagCount(USA_CHARACTER_ACTION) <= 0)
	{
		AnimInstance->Montage_StopGroupByName(0.0f, ItemPickUpAnimMontage->GetGroupName());
		AnimInstance->Montage_Play(ItemPickUpAnimMontage, ItemPickUpAnimMontageRate, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}
}

//void AUSAItemBase::SetPickableActorOwner(AUSACharacterBase* InCharacter)
//{
//
//}
//
//void AUSAItemBase::OnRep_PickableActorOwner(AUSACharacterBase* PrevCharacter)
//{
//
//}

void AUSAItemBase::ActiveUSAItem(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	K2_ActiveUSAItem(InASC, InCharacter);
}

void AUSAItemBase::PlayUseAnimationMontageInUSACharacter(UAbilitySystemComponent* InASC, AUSACharacterBase* InCharacter)
{
	if (IsValid(InCharacter) == false)
	{
		return;
	}

	InCharacter->StopAndPlayAnimMontageByGroup(ItemUseAnimMontage, ItemUseAnimMontageRate);

	//UAnimInstance* AnimInstance = (InCharacter->GetMesh()) ? InCharacter->GetMesh()->GetAnimInstance() : nullptr;

	//if (IsValid(ItemUseAnimMontage) == true
	//	&& IsValid(AnimInstance) == true
	//	/*&& IsValid(InASC) == true
	//	&& InASC->GetGameplayTagCount(USA_CHARACTER_ACTION) <= 0*/)
	//{
	//	AnimInstance->Montage_StopGroupByName(0.0f, ItemUseAnimMontage->GetGroupName());
	//	AnimInstance->Montage_Play(ItemUseAnimMontage, ItemUseAnimMontageRate, EMontagePlayReturnType::MontageLength, 0.0f, false);
	//}
}