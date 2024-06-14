// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USALevelSequenceEnd.h"

#include "Components/TextRenderComponent.h"

#include "GameMode/USAGameModeBase.h"
#include "GameState/USAGameStateBase.h"

AUSALevelSequenceEnd::AUSALevelSequenceEnd()
{
	if (IsValid(LevelSequenceTriggerTextComponent))
	{
		LevelSequenceTriggerTextComponent->SetText(FText::FromString("Level Sequence End"));
	}
}

void AUSALevelSequenceEnd::PlayEndLevelSequence()
{
	if (bIsActivated == true)
	{
		return;
	}

	// 로컬
	if (bIsLocalEvent)
	{
		AUSAGameStateBase* USAGameStateBase = Cast <AUSAGameStateBase>(GetWorld()->GetGameState());

		if (IsValid(USAGameStateBase) == false)
		{
			return;
		}

		USAGameStateBase->PlayLevelSequenceLocal(LevelSequenceActor);
	}
	// 멀티
	else
	{
		AUSAGameModeBase* USAGameModeBase = Cast<AUSAGameModeBase>(GetWorld()->GetAuthGameMode());

		if (IsValid(USAGameModeBase) == false)
		{
			return;
		}

		USAGameModeBase->PlayLevelSequenceToAllPlayer(LevelSequenceActor);
	}

	bIsActivated = true;
}
