// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USALevelSequenceTrigger.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "GameMode/USAGameModeBase.h"
#include "GameState/USAGameStateBase.h"

#include "Player/USAPlayerController.h"

// Sets default values
AUSALevelSequenceTrigger::AUSALevelSequenceTrigger()
{
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Component"));
	RootComponent = TriggerBoxComponent;
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSALevelSequenceTrigger::OnPlayerDetectBoxOverlapBegin);

	if (IsValid(LevelSequenceTriggerTextComponent))
	{
		LevelSequenceTriggerTextComponent->SetupAttachment(RootComponent);
		LevelSequenceTriggerTextComponent->SetText(FText::FromString("Level Sequence Trigger"));
	}
}

// Called when the game starts or when spawned
void AUSALevelSequenceTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUSALevelSequenceTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSALevelSequenceTrigger::OnPlayerDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActivated == true)
	{
		return;
	}

	AUSACharacterPlayer* OtherUSACharacterPlayer = Cast<AUSACharacterPlayer>(OtherActor);

	if (IsValid(OtherUSACharacterPlayer) == false)
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

		AUSAPlayerController* USAPlayerController = OtherUSACharacterPlayer->GetController<AUSAPlayerController>();

		if (IsValid(USAPlayerController) == false)
		{
			return;
		}

		if (USAPlayerController->IsLocalPlayerController() == false)
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

