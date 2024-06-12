// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/USAHUD.h"

#include "GameFramework/PlayerController.h"
#include "Player/USAPlayerController.h"

#include "GameFramework/Character.h"
#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"


void AUSAHUD::BeginPlay()
{
	Super::BeginPlay();


}

//

void AUSAHUD::InitPlayerHUD(AUSACharacterPlayer* InPlayer)
{
    if (IsValid(InPlayer) == false)
    {
        return;
    }

    //if (bIsInitialized == true)
    //{
    //    return;
    //}

    LocalPlayerController = InPlayer->GetController<APlayerController>();
    LocalUSAPlayerController = InPlayer->GetController<AUSAPlayerController>();

    K2_InitPlayerHUD(InPlayer);

    //bIsInitialized = true;
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

void AUSAHUD::ShowHideHowToPlay(bool bIsShowing)
{
    K2_ShowHideHowToPlay(bIsShowing);
}

//

void AUSAHUD::ShowHideMenu(bool bIsShowing)
{
    K2_ShowHideMenu(bIsShowing);
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

void AUSAHUD::UpdateBossHealthRatio(float InHealth, float InMax, float InCurrent)
{
    K2_UpdateBossHealthRatio(InHealth, InMax, InCurrent);
}

//

void AUSAHUD::UpdatePlayerWeapon(int32 InEquipIndex, AUSAWeaponBase* InWeapon)
{
    K2_UpdatePlayerWeapon(InEquipIndex, InWeapon);
}

void AUSAHUD::UpdatePlayerHPBar(float InRatio, float InMax, float InCurrent)
{
    K2_UpdatePlayerHPBar(InRatio, InMax, InCurrent);
}

void AUSAHUD::UpdatePlayerNickname(const FString& InName)
{
    K2_UpdatePlayerNickname(InName);
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

//void AUSAHUD::InitLocalPlayerController(APlayerController* InPlayerController, AUSAPlayerController* InUSAPlayerController)
//{
//    LocalPlayerController = InPlayerController;
//    LocalUSAPlayerController = InUSAPlayerController;
//}

//

