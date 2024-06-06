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


void AUSAGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APawn* ControlledPawn = nullptr;

	if (IsValid(PlayerController) == true
		&& PlayerController->IsLocalPlayerController() == true)
	{
		ControlledPawn = PlayerController->GetPawn();
		
		LocalUSACharacter = Cast<AUSACharacterBase>(ControlledPawn);
		LocalUSAPlayerController = Cast<AUSAPlayerController>(PlayerController);
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

void AUSAGameStateBase::UpdateBossHealthRatio(float InRatio)
{
	CurrentBossHealthRatio = InRatio;

	if (UKismetSystemLibrary::IsServer(GetWorld()) == true
		|| UKismetSystemLibrary::IsStandalone(GetWorld()) == true)
	{
		OnRep_CurrentBossHealthRatio();
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
	K2_UpdateBossHealthRatio(CurrentBossHealthRatio);
}

void AUSAGameStateBase::PlayUserWidgetAnimationP_Panel(bool bIsShowing, bool bIsRaw)
{
	if (IsValid(LocalUSAPlayerController) == false)
	{
		return;
	}

	LocalUSAPlayerController->PlayUserWidgetAnimation_Panel(bIsShowing, bIsRaw);
}


//

void AUSAGameStateBase::SetPlayerControllerInput(bool InActive)
{
	ServerRPC_SetPlayerControllerInput(InActive);
}

void AUSAGameStateBase::ServerRPC_SetPlayerControllerInput_Implementation(bool InActive)
{
	MulticastRPC_SetPlayerControllerInput(InActive);
}

void AUSAGameStateBase::MulticastRPC_SetPlayerControllerInput_Implementation(bool InActive)
{
	SetPlayerControllerInputFinal(InActive);
}

void AUSAGameStateBase::SetPlayerControllerInputFinal(bool InActive)
{
	if (IsValid(LocalUSACharacter) == true
		&& IsValid(LocalUSAPlayerController) == true)
	{
		if (InActive)
		{
			LocalUSACharacter->EnableInput(LocalUSAPlayerController);
		}
		else
		{
			LocalUSACharacter->DisableInput(LocalUSAPlayerController);
		}
	}
}

//

void AUSAGameStateBase::PlayLevelSequenceMulti(ALevelSequenceActor* InLevelSequence)
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
		return;
	}

	//MultiLevelSeqeunceActor = InLevelSequence;

	K2_PlayLevelSequenceMulti(InLevelSequence);
}

void AUSAGameStateBase::PlayLevelSequenceSingle(ALevelSequenceActor* InLevelSequence)
{
	if (InLevelSequence == nullptr
		|| InLevelSequence->SequencePlayer == nullptr)
	{
		return;
	}

	if (IsValid(MultiLevelSeqeunceActor) == true
		&& IsValid(MultiLevelSeqeunceActor->SequencePlayer) == true
		&& MultiLevelSeqeunceActor->SequencePlayer->IsPlaying() == true)
	{
		return;
	}

	//SingleLevelSeqeunceActor = InLevelSequence;

	K2_PlayLevelSequenceSingle(InLevelSequence);
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
}