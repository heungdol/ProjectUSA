// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_CheckCharacterCeiling.h"

#include "AbilitySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UAT_CheckCharacterCeiling* UAT_CheckCharacterCeiling::GetNewAbilityTask_CheckCharacterCeiling
(UGameplayAbility* OwningAbility, ACharacter* InCharacter, float InCharacterHeight, float InCharacterRadius)
{
	UAT_CheckCharacterCeiling* MyObj = NewAbilityTask<UAT_CheckCharacterCeiling>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->DetectCharacterHeight = InCharacterHeight;
	MyObj->DetectCharacterRadius = InCharacterRadius;

	MyObj->bTickingTask = true;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_CheckCharacterCeiling::Activate()
{
	Super::Activate();

	if (MyCharacter == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}
}

void UAT_CheckCharacterCeiling::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MyCharacter == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	float MyCharacterHalfHeight = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float MyCharacterRadius = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float TraceLength = DetectCharacterHeight - (MyCharacterHalfHeight * 2.0f) - MyCharacterRadius;
	TraceLength = FMath::Clamp(TraceLength, 0, TraceLength);

	FVector StartTracePoint = MyCharacter->GetActorLocation()  + FVector::UpVector * MyCharacterHalfHeight;
	FVector EndTracePoint = StartTracePoint + FVector::UpVector * TraceLength;


	TArray <AActor*> IgnoreActors;
	IgnoreActors.Add(GetAvatarActor());

	APawn* Pawn = Cast <APawn>(GetAvatarActor());

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), 
		StartTracePoint, 
		EndTracePoint, 
		DetectCharacterRadius, 
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), 
		false, 
		IgnoreActors, 
		EDrawDebugTrace::None, 
		HitResult, 
		true, 
		FLinearColor::Red, 
		FLinearColor::Green, 
		0.0f);

	if (HitResult.bBlockingHit)
	{
		if (bIsCharacterCeilingDetected == false)
		{
			bIsCharacterCeilingDetected = true;
			BroadcastSimpleDelegate(OnCeilingTrue);
		}
	}
	else
	{
		if (bIsCharacterCeilingDetected == true)
		{
			bIsCharacterCeilingDetected = false;
			BroadcastSimpleDelegate(OnCeilingFalse);
		}
	}
}
