// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerController.h"

#include "Character/USACharacterBase.h"


void AUSAPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // 시작시 호출
    if (IsLocalController() == true)
    {
        if (AUSACharacterBase* USACharacterOwner = Cast<AUSACharacterBase>(GetPawn()))
        {
            USACharacterOwner->K2_OnUSAUpdateHUDUserWidget(PlayerHUDUserWidget);
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
        if (AUSACharacterBase* USACharacterOwner = Cast<AUSACharacterBase>(GetPawn()))
        {
            //UE_LOG(LogTemp, Warning, TEXT("Player has fully joined the game"));
            //USACharacterOwner->SetWeaponDetectBoxComponentActive(true);
            USACharacterOwner->SetCurrentWeaponsUsingStartWeaponClassList();
            USACharacterOwner->K2_OnUSAUpdateHUDUserWidget(PlayerHUDUserWidget);
        }
    }
}