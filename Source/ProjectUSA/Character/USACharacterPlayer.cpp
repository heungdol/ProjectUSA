// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterPlayer.h"

#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Player/USAPlayerState.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "ProjectUSA.h"



AUSACharacterPlayer::AUSACharacterPlayer()
{
	ASC = nullptr;
}

void AUSACharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
}

void AUSACharacterPlayer::Look(const FInputActionValue& Value)
{
	AUSAPlayerState* USAPlayerState = GetPlayerState <AUSAPlayerState>();

	if (USAPlayerState == nullptr)
	{
		Super::Look(Value);
		return;
	}
	
	float LookSensitivityRatio = USAPlayerState->GetLookSensitivityRatio();

	//USA_LOG(LogTemp, Log, TEXT("Current Look Sensitivity Ratio: %f"), LookSensitivityRatio);

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivityRatio);
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivityRatio);
	}
}

void AUSACharacterPlayer::SetupGAS()
{
	AUSAPlayerState* USAPlayerState = GetPlayerState <AUSAPlayerState>();

	if (USAPlayerState != nullptr)
	{
		ASC = USAPlayerState->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(USAPlayerState, this);
	}

	Super::SetupGAS();

	//AUSAPlayerState* USAPlayerState = GetPlayerState <AUSAPlayerState>();
	//UUSAAttributeSet* USAAttributeSet = nullptr;

	//if (USAPlayerState != nullptr)
	//{
	//	ASC = USAPlayerState->GetAbilitySystemComponent();
	//	ASC->InitAbilityActorInfo(USAPlayerState, this);
	//}

	//if (ASC != nullptr)
	//{
	//	USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	//}

	//if (USAAttributeSet != nullptr)
	//{
	//	USAAttributeSet->OnOutOfHealth.AddUObject(this, &AUSACharacterBase::OnUSADeath);
	//}

	//Super::SetupGAS();
}
