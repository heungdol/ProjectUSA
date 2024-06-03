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

void AUSACharacterNonPlayer::OnRep_bIsASCInitialized()
{
	Super::OnRep_bIsASCInitialized();

	PostSetupGAS();

	SetupAttributeSet();

	BeginStartAbilities();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());

	CheckCharacterByGameplayTags();
}

void AUSACharacterNonPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	Super::SetupGAS();
}

void AUSACharacterNonPlayer::UpdateCurrentTargetableActor()
{
	// 보통 BT에서 처리
	
	// ... 

}

void AUSACharacterNonPlayer::OnGameplayTagCallback_Dead(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::OnGameplayTagCallback_Dead(CallbackTag, NewCount);

	if (NewCount > 0)
	{
		SetLifeSpan(LifeSpanAfterDead);
	}
	else
	{

	}
}
