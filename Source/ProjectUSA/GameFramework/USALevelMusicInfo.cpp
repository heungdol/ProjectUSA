// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/USALevelMusicInfo.h"

#include "GameState/USAGameStateBase.h"

// Sets default values
AUSALevelMusicInfo::AUSALevelMusicInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUSALevelMusicInfo::BeginPlay()
{
	Super::BeginPlay();

	AUSAGameStateBase* USAGameStateBase = GetWorld()->GetGameState<AUSAGameStateBase>();

	if (IsValid(USAGameStateBase))
	{
		USAGameStateBase->SetMusicInfoAndPlay(BackgroundSound, BackgroundMusic_Peaseful, BackgroundMusic_Boss);
	}
	
}

// Called every frame
void AUSALevelMusicInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

