// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectUSAGameMode.h"
#include "ProjectUSACharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectUSAGameMode::AProjectUSAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
