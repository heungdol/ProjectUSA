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

#include "Components/AudioComponent.h"

AUSAGameStateBase::AUSAGameStateBase()
{
	AudioComponent_BGS = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundSound"));
	AudioComponent_BGS->bAutoActivate = false;

	AudioComponent_BGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusic"));
	AudioComponent_BGM->bAutoActivate = false;
}

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

	if (IsValid(AudioComponent_BGM))
	{
		AudioComponent_BGM->Stop();

		if (bIsShowingBossHPBar)
		{
			AudioComponent_BGM->Sound = BackgroundMusic_Boss;
		}
		else
		{
			AudioComponent_BGM->Sound = BackgroundMusic_Peaseful;
		}

		AudioComponent_BGM->Play();
	}
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

void AUSAGameStateBase::PlayUserWidgetAnimationLocal_Panel(bool bIsShowing, bool bIsRaw)
{
	if (IsValid(LocalUSAHUD) == false)
	{
		return;
	}

	LocalUSAHUD->PlayUserWidgetAnimation_Panel(bIsShowing, bIsRaw);
}

void AUSAGameStateBase::OnPlayerWinning()
{
	MulticastRPC_OnPlayerWinning();
}

void AUSAGameStateBase::MulticastRPC_OnPlayerWinning_Implementation()
{
	K2_OnPlayerWinning();
}

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

	K2_PlayLevelSequenceLocal(InLevelSequence);
}

void AUSAGameStateBase::SetMusicInfoAndPlay(USoundBase* InBackgroundSound, USoundBase* InBackgroundMusic_Peaseful, USoundBase* InBackgroundMusic_Boss)
{
	BackgroundSound = InBackgroundSound;
	BackgroundMusic_Peaseful = InBackgroundMusic_Peaseful;
	BackgroundMusic_Boss = InBackgroundMusic_Boss;

	if (IsValid(AudioComponent_BGS))
	{
		AudioComponent_BGS->Stop();
		
		AudioComponent_BGS->Sound = BackgroundSound;
		
		AudioComponent_BGS->Play();
	}

	if (IsValid(AudioComponent_BGM))
	{
		AudioComponent_BGM->Stop();

		if (bIsShowingBossHPBar)
		{
			AudioComponent_BGM->Sound = BackgroundMusic_Boss;
		}
		else
		{
			AudioComponent_BGM->Sound = BackgroundMusic_Peaseful;
		}

		AudioComponent_BGM->Play();
	}
}

//

void AUSAGameStateBase::PlayUserWidgetAnmation_Panel(bool InActive, bool IsRaw)
{
	ServerRPC_PlayUserWidgetAnmation_Panel(InActive, IsRaw);
}

void AUSAGameStateBase::ServerRPC_PlayUserWidgetAnmation_Panel_Implementation(bool InActive, bool IsRaw)
{
	MulticastRPC_PlayUserWidgetAnmation_Panel(InActive, IsRaw);
}

void AUSAGameStateBase::MulticastRPC_PlayUserWidgetAnmation_Panel_Implementation(bool InActive, bool IsRaw)
{
	PlayUserWidgetAnimationLocal_Panel(InActive, IsRaw);
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