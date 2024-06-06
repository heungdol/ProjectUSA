// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/USAGameModeBase.h"

#include "GameState/USAGameStateBase.h"
#include "GameFramework/PlayerController.h"

#include "Character/USACharacterBase.h"

#include "LevelSequenceActor.h"

#include "Kismet/GameplayStatics.h"


void AUSAGameModeBase::SetAllPlayerControllerInput(bool InActive)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->SetPlayerControllerInput(InActive);
	}
}

void AUSAGameModeBase::PlayLevelSequenceToAllPlayer(ALevelSequenceActor* InLevelSequnce)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->PlayLevelSequence(InLevelSequnce);
	}
}

void AUSAGameModeBase::SetBossUSACharacter(AUSACharacterBase* InCharacter)
{
	BossUSACharacter = InCharacter;

	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == false)
	{
		return;
	}

	if (IsValid(BossUSACharacter) == true)
	{
		USAGameState->ShowHideBossHPBar(true);
	}
	else
	{
		USAGameState->ShowHideBossHPBar(false);
	}
}
