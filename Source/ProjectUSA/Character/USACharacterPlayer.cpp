// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterPlayer.h"

void AUSACharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = CastChecked <APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
}
