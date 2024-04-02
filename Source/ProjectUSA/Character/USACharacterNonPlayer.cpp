// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterNonPlayer.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"


AUSACharacterNonPlayer::AUSACharacterNonPlayer()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
}

void AUSACharacterNonPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		return;
	}

	ASC->InitAbilityActorInfo(this, this);

	Super::SetupGAS();
}
