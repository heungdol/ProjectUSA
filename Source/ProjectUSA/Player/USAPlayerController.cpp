// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "Weapon/USAWeaponBase.h"
#include "Item/USAItemBase.h"

#include "HUD/USAHUD.h"


void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 시작시 호출
    if (IsLocalController() == true)
    {
        AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn());
        AUSAHUD* USAHUD = Cast<AUSAHUD>(GetHUD());

        if (USACharacterOwner)
        {
            USACharacterOwner->InitPlayerController();
        }

        if (USACharacterOwner && USAHUD)
        {
            USAHUD->InitCharacterHUD(USACharacterOwner);
        }
    }

    // HUD 가져오기
    LocalUSAHUD = Cast<AUSAHUD>(GetHUD());
}

void AUSAPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();

    if (IsLocalController() == true)
    {
        // 클라이언트가 완전히 접속되었음을 알리는 코드
        
        AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn());
        AUSAHUD* USAHUD = Cast<AUSAHUD>(GetHUD());

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
    }
}