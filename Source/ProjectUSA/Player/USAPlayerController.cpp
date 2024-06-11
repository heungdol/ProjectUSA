// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "Weapon/USAWeaponBase.h"
#include "Item/USAItemBase.h"

#include "HUD/USAHUD.h"

#include "GameInstance/USAGameInstance.h"

#include "Player/USAPlayerState.h"

void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();

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

        if (USACharacterOwner && USAHUD)
        {
            USAHUD->InitCharacterHUD(USACharacterOwner);
        }

        if (USACharacterOwner && USAPlayerState && USAGameInstance)
        {
            USACharacterOwner->ChangeCharacterName(USAGameInstance->GetPlayerNickByIndex(USAPlayerState->GetPlayerIndex()));
        }
    }

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

    if (IsLocalController() == true)
    {
        // 클라이언트가 완전히 접속되었음을 알리는 코드
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
            USAHUD->InitCharacterHUD(USACharacterOwner);
        }

        if (USACharacterOwner && USAPlayerState && USAGameInstance)
        {
            USACharacterOwner->ChangeCharacterName(USAGameInstance->GetPlayerNickByIndex(USAPlayerState->GetPlayerIndex()));
        }
    }
}