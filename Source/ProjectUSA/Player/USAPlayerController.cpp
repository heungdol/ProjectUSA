// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "Weapon/USAWeaponBase.h"
#include "Item/USAItemBase.h"

#include "HUD/USAHUD.h"

#include "GameInstance/USAGameInstance.h"

#include "Player/USAPlayerState.h"

#include "Kismet/GameplayStatics.h"

#include "Trigger/USALevelSequenceBegin.h"


void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn());
    AUSAHUD* USAHUD = Cast<AUSAHUD>(GetHUD());
    UUSAGameInstance* USAGameInstance = Cast <UUSAGameInstance>(GetGameInstance());
    AUSAPlayerState* USAPlayerState = GetPlayerState<AUSAPlayerState>();

    // HUD 가져오기
    LocalUSAHUD = Cast<AUSAHUD>(GetHUD());
}

void AUSAPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();

    AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn());
    AUSAHUD* USAHUD = Cast<AUSAHUD>(GetHUD());
    UUSAGameInstance* USAGameInstance = Cast <UUSAGameInstance>(GetGameInstance());
    AUSAPlayerState* USAPlayerState = GetPlayerState<AUSAPlayerState>();

    // 시작시 호출
    if (IsLocalController() == true)
    {
        if (USACharacterOwner)
        {
            USACharacterOwner->InitPlayerController();
        }

        if (USACharacterOwner)
        {
            USACharacterOwner->SetCurrentWeaponsUsingStartWeaponClassList();
        }

        if (USACharacterOwner && USAHUD)
        {
            USAHUD->InitPlayerHUD(USACharacterOwner);
        }

        // 첫 인트로가 없는 경우
        if (IsValid(UGameplayStatics::GetActorOfClass(GetWorld(), AUSALevelSequenceBegin::StaticClass())) == false)
        {
            USAHUD->PlayUserWidgetAnimation_Panel(true, false);
        }
    }
}