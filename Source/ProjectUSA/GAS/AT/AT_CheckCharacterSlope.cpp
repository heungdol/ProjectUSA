// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_CheckCharacterSlope.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Debug/DebugDrawComponent.h"


UAT_CheckCharacterSlope* UAT_CheckCharacterSlope::GetNewAbilityTask_CheckCharacterSlope
(UGameplayAbility* OwningAbility, ACharacter* InCharacter, float InStartSlopeAngle)
{
	UAT_CheckCharacterSlope* MyObj = NewAbilityTask<UAT_CheckCharacterSlope>(OwningAbility);

	MyObj->MyCharacter = InCharacter;
	MyObj->StartSlopeAngle = InStartSlopeAngle;

	MyObj->bTickingTask = true;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_CheckCharacterSlope::Activate()
{
	Super::Activate();
	
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
}

void UAT_CheckCharacterSlope::TickTask(float DeltaTime)
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

	if (MyCharacter->GetCharacterMovement()->CurrentFloor.bBlockingHit == false)
	{
		SimpleEndAbilityTask();
		return;
	}

	FVector GroundNormal = MyCharacter->GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	FVector GroundRightVector = FVector::CrossProduct(GroundNormal, MyCharacter->GetActorForwardVector());
	FVector GroundFowardVector = FVector::CrossProduct(GroundRightVector, GroundNormal);
	GroundFowardVector.Normalize();

	//APawn* Pawn = Cast <APawn>(GetAvatarActor());

	//if (Pawn != nullptr && Pawn->IsLocallyControlled())
	//{
	//	FVector CharacterLocation = MyCharacter->GetActorLocation();
	//	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundFowardVector * 500, FColor::Red, false, -1.0f, 0U, 10.0f);
	//	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundRightVector * 500, FColor::Green, false, -1.0f, 0U, 10.0f);
	//	DrawDebugLine(GetWorld(), CharacterLocation, CharacterLocation + GroundNormal * 500, FColor::Blue, false, -1.0f, 0U, 10.0f);
	//}

	float Dot = FVector::DotProduct(GroundFowardVector, FVector::DownVector);
	float MinDot = FMath::Cos(StartSlopeAngle);
	
	//UE_LOG(LogTemp, Log, TEXT("%f"), MinDot);
	
	if (Dot > MinDot)
	{
		if (bIsCharacterOnSlopeWithForwardDirection == false)
		{
			bIsCharacterOnSlopeWithForwardDirection = true;
			
			BroadcastSimpleDelegate(OnSlopeTrue);
		}
	}
	else
	{
		if (bIsCharacterOnSlopeWithForwardDirection == true)
		{
			bIsCharacterOnSlopeWithForwardDirection = false;

			BroadcastSimpleDelegate (OnSlopeFalse);
		}
	}
}


void UAT_CheckCharacterSlope::SimpleEndAbilityTask()
{
	BroadcastSimpleDelegate(OnGroundOut);
	
	Super::SimpleEndAbilityTask();
}

