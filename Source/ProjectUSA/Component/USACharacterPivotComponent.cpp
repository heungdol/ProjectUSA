// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/USACharacterPivotComponent.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UUSACharacterPivotComponent::UUSACharacterPivotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUSACharacterPivotComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPivotComponent();
}

void UUSACharacterPivotComponent::InitPivotComponent()
{
	Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = Character->GetMesh();
	if (SkeletalMesh == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(SkeletalMesh, AttachmentTransformRules, PivotSocketName);
}


// Called every frame
void UUSACharacterPivotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Pawn = Cast <APawn>(GetOwner());

	if (Pawn && Pawn->IsLocallyControlled())
	{
		DrawDebugSphere(GetWorld(), GetComponentLocation(), 20.0f, 16, FColor::Magenta);
	}
}

