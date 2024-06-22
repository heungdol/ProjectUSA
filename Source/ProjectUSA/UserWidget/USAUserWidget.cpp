// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/USAUserWidget.h"

#include "Windows/WindowsApplication.h"  // Windows API 사용을 위해 필요
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"


#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "GameFramework/PlayerController.h"
#include "Player/USAPlayerController.h"
#include "Player/USAPlayerState.h"

#include "Character/USACharacterPlayer.h"
#include "Character/USACharacterBase.h"

#include "HUD/USAHUD.h"


void UUSAUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    APlayerController* PlayerController = GetOwningPlayer();

    if (IsValid(PlayerController) == true
        && PlayerController->IsLocalPlayerController() == true)
    {
        APawn* ControlledPawn = nullptr;
        ControlledPawn = PlayerController->GetPawn();

        LocalUSACharacter = Cast<AUSACharacterBase>(ControlledPawn);
        LocalUSAPlayerController = Cast<AUSAPlayerController>(PlayerController);

        if (LocalUSAPlayerController)
        {
            LocalUSAHUD = Cast<AUSAHUD>(LocalUSAPlayerController->GetHUD());
        }
    }

    APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController();
    if (FirstPlayerController)
    {
        USAPlayerState = FirstPlayerController->GetPlayerState<AUSAPlayerState>();
    }
}

void UUSAUserWidget::SimulateClickMouseLeftButtonPressed()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (IsValid(PlayerController) == false
        || PlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, &Input, sizeof(INPUT));
}

void UUSAUserWidget::SimulateClickMouseLeftButtonReleased()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (IsValid(PlayerController) == false
        || PlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;  

    SendInput(1, &Input, sizeof(INPUT));
}

void UUSAUserWidget::SimulateMoveMousePosition(FVector InDelta, float InSpeed, float InDeltaTime)
{
    if (FMath::Abs(InDelta.X) < 0.2f && FMath::Abs(InDelta.Y) < 0.2f)
    {
        return;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (IsValid(PlayerController) == false
        || PlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    FVector FinalDelta = InDelta * InSpeed * InDeltaTime;
    FinalDelta.Y *= -1.0f;

    FVector Final;
    PlayerController->GetMousePosition(Final.X, Final.Y);

    Final.X += FinalDelta.X;
    Final.Y += FinalDelta.Y;
    
    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(PlayerController);

    Final.X = FMath::Clamp(Final.X, ScreenBumpOffset, ScreenSize.X - ScreenBumpOffset);
    Final.Y = FMath::Clamp(Final.Y, ScreenBumpOffset, ScreenSize.Y - ScreenBumpOffset);

    PlayerController->SetMouseLocation(Final.X, Final.Y);
}