// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/USAHUD.h"

#include "GameFramework/PlayerController.h"
#include "Player/USAPlayerController.h"


void AUSAHUD::BeginPlay()
{
	Super::BeginPlay();


}

//

void AUSAHUD::InitCharacterHUD(AUSACharacterPlayer* InPlayer)
{
    K2_InitPlayerHUD(InPlayer);
}

//

void AUSAHUD::PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw)
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

void AUSAHUD::ShowHideCharacterHUD(bool bIsShowing)
{
    K2_ShowHideUserWidget_PlayerHUD(bIsShowing);
}

//

void AUSAHUD::ShowHideBossHPBar(bool bIsShowing)
{
    K2_ShowHideUserWidget_BossHPBar(bIsShowing);
}

void AUSAHUD::UpdateBossName(FName InName)
{
    K2_UpdateBossName(InName);
}

void AUSAHUD::UpdateBossHealthRatio(float InHealth)
{
    K2_UpdateBossHealthRatio(InHealth);
}

//

void AUSAHUD::UpdatePlayerWeapon(EUSAWeaponType InType, AUSAWeaponBase* InWeapon)
{
    K2_UpdatePlayerWeapon(InType, InWeapon);
}

void AUSAHUD::UpdatePlayerHPBar(float InRatio)
{
    K2_UpdatePlayerHPBar(InRatio);
}

//

void AUSAHUD::ChangePlayerItem(TSubclassOf<AUSAItemBase> InItemClass)
{
    K2_ChangePlayerItem(InItemClass);
}

void AUSAHUD::ChangePlayerItemCount(int32 InItemCount)
{
    K2_ChangePlayerItemCount(InItemCount);
}

void AUSAHUD::InitLocalPlayerController(APlayerController* InPlayerController, AUSAPlayerController* InUSAPlayerController)
{
    LocalPlayerController = InPlayerController;
    LocalUSAPlayerController = InUSAPlayerController;
}

//

