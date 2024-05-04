// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterNonPlayer.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Debug/DebugDrawComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "Controller/USAAIController.h"

#include "ProjectUSA.h"


AUSACharacterNonPlayer::AUSACharacterNonPlayer()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject <UUSAAttributeSet>(TEXT("USA Attribute Set"));

	if (ASC != nullptr)
	{
		ASC->SetIsReplicated(true);
	}
}

//void AUSACharacterNonPlayer::OnRep_Controller()
//{
//	Super::OnRep_Controller();
//
//	SetupGAS();
//}

//void AUSACharacterNonPlayer::PossessedBy(AController* NewController)
//{
//	Super::PossessedBy(NewController);
//
//	SetupGAS();
//}

void AUSACharacterNonPlayer::OnRep_bIsASCInitialized()
{
	Super::OnRep_bIsASCInitialized();

	PostSetupGAS();

	SetupAttributeSet();

	BeginStartAbilities();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}

void AUSACharacterNonPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	Super::SetupGAS();
}
