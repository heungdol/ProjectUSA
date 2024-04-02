// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterPlayer.h"

#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

AUSACharacterPlayer::AUSACharacterPlayer()
{
	ASC = nullptr;
}

void AUSACharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = Cast <APlayerController>(NewController);
	if (PlayerController != nullptr)
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AUSACharacterPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		return;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast <IAbilitySystemInterface>(GetPlayerState());
	if (AbilitySystemInterface != nullptr)
	{
		ASC = AbilitySystemInterface->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(Cast<AActor>(GetPlayerState()), this);
	}

	Super::SetupGAS();
}
