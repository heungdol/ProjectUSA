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
	//ASC = nullptr;
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

		if (ASC != nullptr)
		{
			ASC->InitAbilityActorInfo(USAPlayerState, this);
		}
	}

	Super::SetupGAS();
}

//

void AUSACharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 일반 클라에서 수행
	SetupGAS();
}


void AUSACharacterPlayer::OnRep_ASC()
{
	Super::OnRep_ASC();

	PostSetupGAS();

	SetupAttributeSet();

	BeginStartAbilities();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}