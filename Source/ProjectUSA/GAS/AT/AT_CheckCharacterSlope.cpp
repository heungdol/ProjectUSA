// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_CheckCharacterSlope.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Debug/DebugDrawComponent.h"


UAT_CheckCharacterSlope* UAT_CheckCharacterSlope::GetNewAbilityTask
(UGameplayAbility* OwningAbility, ACharacter* InCharacter, float InStartSlopeAngle)
{
	UAT_CheckCharacterSlope* MyObj = NewAbilityTask<UAT_CheckCharacterSlope>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->StartSlopeAngle = InStartSlopeAngle;

	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_CheckCharacterSlope::Activate()
{
	Super::Activate();
	
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_CheckCharacterSlope::OnCancelTaskCallback);
	}

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
}

void UAT_CheckCharacterSlope::TickTask(float DeltaTime)
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

	if (MyCharacter->GetCharacterMovement()->CurrentFloor.bBlockingHit == false)
	{
		OnEndTaskCallback();
		return;
	}

	FVector GroundNormal = MyCharacter->GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	FVector GroundRightVector = FVector::CrossProduct(GroundNormal, MyCharacter->GetActorForwardVector());
	FVector GroundFowardVector = FVector::CrossProduct(GroundRightVector, GroundNormal);
	GroundFowardVector.Normalize();
	


	FVector CharacterLocation = MyCharacter->GetActorLocation();
	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundFowardVector * 500, FColor::Red, false, -1.0f, 0U, 10.0f);
	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundRightVector * 500, FColor::Green, false, -1.0f, 0U, 10.0f);
	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundNormal * 500, FColor::Blue, false, -1.0f, 0U, 10.0f);

	float Dot = FVector::DotProduct(GroundFowardVector, FVector::DownVector);
	float MinDot = FMath::Cos(StartSlopeAngle);
	
	UE_LOG(LogTemp, Log, TEXT("%f"), MinDot);
	
	if (Dot > MinDot)
	{
		if (bIsCharacterOnSlopeWithForwardDirection == false)
		{
			bIsCharacterOnSlopeWithForwardDirection = true;
			OnSlopeTrue.Broadcast();
		}
	}
	else
	{
		if (bIsCharacterOnSlopeWithForwardDirection == true)
		{
			bIsCharacterOnSlopeWithForwardDirection = false;
			OnSlopeFalse.Broadcast();
		}
	}
}

void UAT_CheckCharacterSlope::OnCancelTaskCallback()
{
	EndTask();
}

void UAT_CheckCharacterSlope::OnEndTaskCallback()
{
	OnGroundOut.Broadcast();

	EndTask();
}

