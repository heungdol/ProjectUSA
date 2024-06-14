// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/USAGameModeBase.h"

#include "GameFramework/PlayerController.h"

#include "GameState/USAGameStateBase.h"
#include "Player/USAPlayerState.h"
#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "LevelSequenceActor.h"

#include "GameFramework/PlayerState.h"

#include "GameFramework/PlayerStart.h"
#include "Engine/PlayerStartPIE.h"

#include "Player/USAPlayerStart.h"

#include "GameInstance/USAGameInstance.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/BlueprintSupport.h"
#include "Engine/GameInstance.h"
#include "Engine/ServerStatReplicator.h"
#include "GameFramework/GameNetworkManager.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LevelScriptActor.h"
#include "Misc/CommandLine.h"
#include "Misc/PackageName.h"
#include "Net/OnlineEngineInterface.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Engine/NetConnection.h"
#include "Engine/ChildConnection.h"
#include "Engine/PlayerStartPIE.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"


AActor* AUSAGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	UWorld* World = GetWorld();

	// 체크포인트
	AUSAPlayerController* USAPlayerController = Cast<AUSAPlayerController>(Player);

	// USA를 안가지면 기존 함수를 수행
	if (IsValid(USAPlayerController) == false)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	int32 DesiredCheckpointIndex = -1;
	if (USAPlayerController && PlayerControllerCheckpointMapList.Contains(USAPlayerController))
	{
		DesiredCheckpointIndex = PlayerControllerCheckpointMapList[USAPlayerController];
	}

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		AUSAPlayerStart* USAPlayerStart = Cast <AUSAPlayerStart>(PlayerStart);

		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		else
		{
			if (IsValid(USAPlayerStart) == false)
			{
				continue;
			}

			// 인덱스가 맞는 플레이어 스타트들만 고르기
			if (USAPlayerStart->GetPlayerStartIndex() != DesiredCheckpointIndex)
			{
				continue;
			}

			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPoints.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPoints.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == nullptr)
	{
		if (UnOccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}
	return FoundPlayerStart;
}

bool AUSAGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AUSAGameModeBase::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
	Super::InitStartSpot_Implementation(StartSpot, NewPlayer);

	if (StartSpot && NewPlayer->GetPawn())
	{
		NewPlayer->GetPawn()->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
	}
}

void AUSAGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUSAPlayerState* PlayerState = Cast <AUSAPlayerState>(NewPlayer->PlayerState);
	UUSAGameInstance* USAGameInstance = Cast<UUSAGameInstance>(GetGameInstance());

	if (PlayerState && USAGameInstance)
	{
		PlayerState->SetPlayerName(USAGameInstance->GetPlayerNickByIndex(CurrentPlayerStackCount));
		
		CurrentPlayerStackCount += 1;
	}
}

//

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

void AUSAGameModeBase::UpdateBossHealthRatio(float InRatio, float InMax, float InCurrent)
{
	AUSAGameStateBase* USAGameState = GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameState) == true)
	{
		USAGameState->UpdateBossHealthRatio(InRatio, InMax, InCurrent);
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
		USAGameState->PlayLevelSequence(InLevelSequnce);
	}
}

void AUSAGameModeBase::UpdatePlayerControllerCheckpoint(AUSAPlayerController* InPlayer, int32 InCheckpointIndex)
{
	if (PlayerControllerCheckpointMapList.Contains(InPlayer) == false)
	{
		PlayerControllerCheckpointMapList.Add({ InPlayer, -1 });
	}

	PlayerControllerCheckpointMapList[InPlayer] = InCheckpointIndex;
}

//void AUSAGameModeBase::RestartUSAPlayer(AUSAPlayerController* NewPlayer)
//{
//	AUSAPlayerController* USAPlayerController = Cast <AUSAPlayerController>(NewPlayer);
//
//	if (IsValid(USAPlayerController) == false)
//	{
//		return;
//	}
//
//	RestartPlayer(USAPlayerController);
//}
