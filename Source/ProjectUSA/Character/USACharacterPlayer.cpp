// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterPlayer.h"

#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Player/USAPlayerState.h"

AUSACharacterPlayer::AUSACharacterPlayer()
{
	ASC = nullptr;
}

void AUSACharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
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
}
