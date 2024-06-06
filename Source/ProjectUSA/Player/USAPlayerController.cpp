// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "Weapon/USAWeaponBase.h"


void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 시작시 호출
    if (IsLocalController() == true)
    {
        if (AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn()))
        {
            K2_InitPlayerHUD(USACharacterOwner);
        }
    }
}

void AUSAPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();

    if (IsLocalController() == true)
    {
        // 클라이언트가 완전히 접속되었음을 알리는 코드
        //         
        if (AUSACharacterPlayer* USACharacterOwner = Cast<AUSACharacterPlayer>(GetPawn()))
        {
            USACharacterOwner->SetCurrentWeaponsUsingStartWeaponClassList();
            K2_InitPlayerHUD(USACharacterOwner);
        }
    }
}

//

void AUSAPlayerController::PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw)
{
    if (bIsShowing)
    {
        if (bIsRaw)
        {
            K2_PlayUserWidgetAnimation_PanelWhite();
        }
        else
        {
            K2_PlayUserWidgetAnimation_PanelShow();
        }
    }
    else
    {
        if (bIsRaw)
        {
            K2_PlayUserWidgetAnimation_PanelBlack();
        }
        else
        {
            K2_PlayUserWidgetAnimation_PanelHide();
        }
    }
}

//

void AUSAPlayerController::ShowHideCharacterHUD(bool bIsShowing)
{
    K2_ShowHideUserWidget_PlayerHUD(bIsShowing);
}

//

void AUSAPlayerController::ShowHideBossHPBar(bool bIsShowing)
{
    K2_ShowHideUserWidget_BossHPBar(bIsShowing);
}

void AUSAPlayerController::UpdateBossName(FName InName)
{
    K2_UpdateBossName(InName);
}

void AUSAPlayerController::UpdateBossHealthRatio(float InHealth)
{
    K2_UpdateBossHealthRatio(InHealth);
}

//

void AUSAPlayerController::UpdatePlayerWeapon(EUSAWeaponType InType, AUSAWeaponBase* InWeapon)
{
    K2_UpdatePlayerWeapon(InType, InWeapon);
}

void AUSAPlayerController::UpdatePlayerHPBar(float InRatio)
{
    K2_UpdatePlayerHPBar(InRatio);
}

//
