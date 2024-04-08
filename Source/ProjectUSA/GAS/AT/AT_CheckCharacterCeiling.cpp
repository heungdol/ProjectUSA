// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_CheckCharacterCeiling.h"

#include "AbilitySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UAT_CheckCharacterCeiling* UAT_CheckCharacterCeiling::GetNewAbilityTask
(UGameplayAbility* OwningAbility, ACharacter* InCharacter, float InCharacterHeight, float InCharacterRadius)
{
	UAT_CheckCharacterCeiling* MyObj = NewAbilityTask<UAT_CheckCharacterCeiling>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->CharacterHeight = InCharacterHeight;
	MyObj->CharacterRadius = InCharacterRadius;

	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_CheckCharacterCeiling::Activate()
{
	Super::Activate();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_CheckCharacterCeiling::OnCancelTaskCallback);
	}

	if (MyCharacter == nullptr)
	{
		OnCancelTaskCallback();
	}
}

void UAT_CheckCharacterCeiling::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MyCharacter == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	if (MyCharacter->GetCharacterMovement() == nullptr)
	{
		OnCancelTaskCallback();
		return;
	}

	float TraceLength = CharacterHeight - MyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector StartTracePoint = MyCharacter->GetActorLocation();
	FVector EndTracePoint = StartTracePoint + FVector::UpVector * TraceLength;


	TArray <AActor*> IgnoreActors;
	IgnoreActors.Add(GetAvatarActor());

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld()
		, StartTracePoint
		, EndTracePoint
		, CharacterRadius
		, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility)
		, false
		, IgnoreActors
		, EDrawDebugTrace::ForDuration
		, HitResult
		, true
		, FLinearColor::Red
		, FLinearColor::Green
		, 0.1f);

	if (HitResult.bBlockingHit)
	{
		if (bIsCharacterCeilingDetected == false)
		{
			bIsCharacterCeilingDetected = true;
			OnCeilingTrue.Broadcast();
		}
	}
	else
	{
		if (bIsCharacterCeilingDetected == true)
		{
			bIsCharacterCeilingDetected = false;
			OnCeilingFalse.Broadcast();
		}
	}
}

void UAT_CheckCharacterCeiling::OnCancelTaskCallback()
{
	EndTask();
}

void UAT_CheckCharacterCeiling::OnEndTaskCallback()
{
	EndTask();
}
