// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/USAGameModeBase.h"

#include "GameFramework/PlayerController.h"

#include "GameState/USAGameStateBase.h"
#include "Player/USAPlayerState.h"
#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "LevelSequenceActor.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerState.h"


void AUSAGameModeBase::RestartPlayer(AController* NewPlayer)
{
	//Super::RestartPlayer(NewPlayer);

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AActor* StartSpot = FindPlayerStart(NewPlayer);

	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}
	}

	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);

}

void AUSAGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	//Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayerAtPlayerStart: Player start not found"));
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnFor(NewPlayer, StartSpot);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);

		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}
}

void AUSAGameModeBase::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	//Super::RestartPlayerAtTransform(NewPlayer, SpawnTransform);

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform: Tried to restart a spectator-only player!"));
		return;
	}

	FRotator SpawnRotation = SpawnTransform.GetRotation().Rotator();

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}
}

AActor* AUSAGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

AActor* AUSAGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}

void AUSAGameModeBase::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
	Super::InitStartSpot_Implementation(StartSpot, NewPlayer);

	if (StartSpot && NewPlayer->GetPawn())
	{
		NewPlayer->GetPawn()->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
	}
}

bool AUSAGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return Super::ShouldSpawnAtStartSpot(Player);
}

void AUSAGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	//Super::FinishRestartPlayer(NewPlayer, StartRotation);

	NewPlayer->Possess(NewPlayer->GetPawn());
	
	// If the Pawn is destroyed as part of possession we have to abort
	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Set initial control rotation to starting rotation rotation
		NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

		FRotator NewControllerRot = StartRotation;
		NewControllerRot.Roll = 0.f;
		NewPlayer->SetControlRotation(NewControllerRot);

		SetPlayerDefaults(NewPlayer->GetPawn());

		K2_OnRestartPlayer(NewPlayer);
	}
}


void AUSAGameModeBase::SetAllPlayerControllerInput(bool InActive)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUSACharacterPlayer::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		if (AUSACharacterPlayer* USACharacterPlayer = Cast <AUSACharacterPlayer> (Actor))
		{
			USACharacterPlayer->SetPlayerControllerInput(InActive);
		}
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

void AUSAGameModeBase::UpdateBossHealthRatio(float InRatio)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->UpdateBossHealthRatio(InRatio);
	}
}

void AUSAGameModeBase::UpdateBossName(FName InName)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->UpdateBossName(InName);
	}
}

void AUSAGameModeBase::PlayLevelSequenceToAllPlayer(ALevelSequenceActor* InLevelSequnce)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->PlayLevelSequenceMulti(InLevelSequnce);
	}
}

void AUSAGameModeBase::RestartUSAPlayer(AUSAPlayerController* NewPlayer)
{
	AUSAPlayerController* USAPlayerController = Cast <AUSAPlayerController>(NewPlayer);

	if (IsValid(USAPlayerController) == false)
	{
		return;
	}

	AUSACharacterPlayer* USACharacterPlayer = Cast <AUSACharacterPlayer >(NewPlayer->GetPawn());

	if (IsValid(USACharacterPlayer) == false)
	{
		return;
	}

	K2_RestartUSAPlayer(USAPlayerController, USACharacterPlayer);
}
