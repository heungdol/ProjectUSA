// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "Weapon/USAWeaponBase.h"
#include "Item/USAItemBase.h"

#include "HUD/USAHUD.h"

#include "Windows/WindowsApplication.h"  // Windows API 사용을 위해 필요
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"


void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 시작시 호출
    if (IsLocalController() == true)
    {
        AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn());
        AUSAHUD* USAHUD = Cast<AUSAHUD>(GetHUD());

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
            USACharacterOwner->SetCurrentWeaponsUsingStartWeaponClassList();
        }

        if (USACharacterOwner && USAHUD)
        {
            USAHUD->InitCharacterHUD(USACharacterOwner);
        }
    }
}

void AUSAPlayerController::SimulateClickMouseLeftButtonPressed()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, &Input, sizeof(INPUT));
}

void AUSAPlayerController::SimulateClickMouseLeftButtonReleased()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &Input, sizeof(INPUT));
}

void AUSAPlayerController::SimulateMoveMousePosition(FVector InDelta, float InSpeed, float InDeltaTime)
{
    if (FMath::Abs(InDelta.X) < 0.2f && FMath::Abs(InDelta.Y) < 0.2f)
    {
        return;
    }

    FVector FinalDelta = InDelta * InSpeed * InDeltaTime;
    FinalDelta.Y *= -1.0f;

    FVector Final;
    GetMousePosition(Final.X, Final.Y);

    Final.X += FinalDelta.X;
    Final.Y += FinalDelta.Y;

    SetMouseLocation(Final.X, Final.Y);
}
