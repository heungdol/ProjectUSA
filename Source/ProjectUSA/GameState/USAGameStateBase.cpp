// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/USAGameStateBase.h"

#include "GameFramework/PlayerController.h"

#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "MovieSceneSequencePlayer.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "Player/USAPlayerController.h"
#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"
#include "HUD/USAHUD.h"

#include "Kismet/GameplayStatics.h"
#include "Trigger/USALevelSequenceBegin.h"

void AUSAGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APawn* ControlledPawn = nullptr;

	if (IsValid(PlayerController) == true
		&& PlayerController->IsLocalPlayerController() == true)
	{
		ControlledPawn = PlayerController->GetPawn();
		
		LocalUSACharacter = Cast<AUSACharacterPlayer>(ControlledPawn);
		LocalUSAPlayerController = Cast<AUSAPlayerController>(PlayerController);

		if (LocalUSAPlayerController)
		{
			LocalUSAHUD = Cast<AUSAHUD>(LocalUSAPlayerController->GetHUD());
		}
	}
}

void AUSAGameStateBase::ShowHideBossHPBar(bool bIsShowing)
{
	bIsShowingBossHPBar = bIsShowing;

	if (UKismetSystemLibrary::IsServer(GetWorld()) == true
		|| UKismetSystemLibrary::IsStandalone(GetWorld()) == true)
	{
		OnRep_bIsShowingBossHPBar();
	}
}

void AUSAGameStateBase::UpdateBossName(FName InName)
{
	CurrentBossName = InName;

	if (UKismetSystemLibrary::IsServer(GetWorld()) == true
		|| UKismetSystemLibrary::IsStandalone(GetWorld()) == true)
	{
		OnRep_CurrentBossName();
	}
}

void AUSAGameStateBase::UpdateBossHealthRatio(float InRatio, float InMax, float InCurrent)
{
	CurrentBossHealthRatio = InRatio;
	CurrentBossHealth = InCurrent;
	MaxBossHealth = InMax;

	if (UKismetSystemLibrary::IsServer(GetWorld()) == true
		|| UKismetSystemLibrary::IsStandalone(GetWorld()) == true)
	{
		OnRep_CurrentBossHealthRatio();
		OnRep_CurrentBossHealth();
	}
}

void AUSAGameStateBase::OnRep_bIsShowingBossHPBar()
{
	K2_ShowHideBossHPBar(bIsShowingBossHPBar);
}

void AUSAGameStateBase::OnRep_CurrentBossName()
{
	K2_UpdateBossName(CurrentBossName);
}

void AUSAGameStateBase::OnRep_CurrentBossHealthRatio()
{
	K2_UpdateBossHealthRatio(CurrentBossHealthRatio, MaxBossHealth, CurrentBossHealth);
}

void AUSAGameStateBase::OnRep_CurrentBossHealth()
{
	K2_UpdateBossHealthRatio(CurrentBossHealthRatio, MaxBossHealth, CurrentBossHealth);
}

void AUSAGameStateBase::PlayUserWidgetAnimationP_Panel(bool bIsShowing, bool bIsRaw)
{
	if (IsValid(LocalUSAHUD) == false)
	{
		return;
	}

	LocalUSAHUD->PlayUserWidgetAnimation_Panel(bIsShowing, bIsRaw);
}


//

//void AUSAGameStateBase::SetPlayerControllerInput(bool InActive)
//{
//	ServerRPC_SetPlayerControllerInput(InActive);
//}
//
//void AUSAGameStateBase::ServerRPC_SetPlayerControllerInput_Implementation(bool InActive)
//{
//	MulticastRPC_SetPlayerControllerInput(InActive);
//}
//
//void AUSAGameStateBase::MulticastRPC_SetPlayerControllerInput_Implementation(bool InActive)
//{
//	SetPlayerControllerInputFinal(InActive);
//}
//
//void AUSAGameStateBase::SetPlayerControllerInputFinal(bool InActive)
//{
//	if (IsValid(LocalUSACharacter) == true
//		&& IsValid(LocalUSAPlayerController) == true)
//	{
//		if (InActive)
//		{
//			LocalUSACharacter->EnableInput(LocalUSAPlayerController);
//		}
//		else
//		{
//			LocalUSACharacter->DisableInput(LocalUSAPlayerController);
//		}
//	}
//}

//

void AUSAGameStateBase::PlayLevelSequence(ALevelSequenceActor* InLevelSequence)
{
	ServerRPC_PlayLevelSequence(InLevelSequence);
}

void AUSAGameStateBase::ServerRPC_PlayLevelSequence_Implementation(ALevelSequenceActor* InLevelSequence)
{
	MulticastRPC_PlayLevelSequence(InLevelSequence);
}

void AUSAGameStateBase::MulticastRPC_PlayLevelSequence_Implementation(ALevelSequenceActor* InLevelSequence)
{
	if (InLevelSequence == nullptr
		|| InLevelSequence->SequencePlayer == nullptr)
	{
		K2_ResetLevelSequence(true, true);

		return;
	}

	//LevelSeqeunceActor = InLevelSequence;

	K2_PlayLevelSequence(InLevelSequence);
}

void AUSAGameStateBase::PlayLevelSequenceLocal(ALevelSequenceActor* InLevelSequence)
{
	if (InLevelSequence == nullptr
		|| InLevelSequence->SequencePlayer == nullptr)
	{
		K2_ResetLevelSequenceLocal(true, true);
		return;
	}

	if (IsValid(LevelSeqeunceActor) == true
		&& IsValid(LevelSeqeunceActor->SequencePlayer) == true
		&& LevelSeqeunceActor->SequencePlayer->IsPlaying() == true)
	{
		return;
	}

	//LocalLevelSeqeunceActor = InLevelSequence;

	K2_PlayLevelSequenceLocal(InLevelSequence);
}

//

void AUSAGameStateBase::PlayLevelSequenceCore(ALevelSequenceActor* InLevelSequence)
{
	if (InLevelSequence == nullptr
		|| InLevelSequence->SequencePlayer == nullptr)
	{
		return;
	}

	InLevelSequence->SequencePlayer->Play();
}


//

void AUSAGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUSAGameStateBase, bIsShowingBossHPBar);
	DOREPLIFETIME(AUSAGameStateBase, CurrentBossName);
	DOREPLIFETIME(AUSAGameStateBase, CurrentBossHealthRatio);
	DOREPLIFETIME(AUSAGameStateBase, CurrentBossHealth);
}