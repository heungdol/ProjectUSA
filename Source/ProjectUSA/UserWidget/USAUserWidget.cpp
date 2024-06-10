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

#include "Character/USACharacterBase.h"
#include "Character/USACharacterPlayer.h"

#include "HUD/USAHUD.h"


void UUSAUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    APlayerController* PlayerController = GetOwningPlayer();
    APawn* ControlledPawn = nullptr;

    if (IsValid(PlayerController) == true
        && PlayerController->IsLocalPlayerController() == true)
    {
        ControlledPawn = PlayerController->GetPawn();

        LocalUSACharacter = Cast<AUSACharacterBase>(ControlledPawn);
        LocalUSAPlayerController = Cast<AUSAPlayerController>(PlayerController);

        if (LocalUSAPlayerController)
        {
            LocalUSAHUD = Cast<AUSAHUD>(LocalUSAPlayerController->GetHUD());
        }
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
    
    FVector ScreenSize;

    UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, ScreenSize.X, ScreenSize.Y);

    Final.X = FMath::Clamp(Final.X, 0, ScreenSize.X);
    Final.Y = FMath::Clamp(Final.Y, 0, ScreenSize.Y);

    PlayerController->SetMouseLocation(Final.X, Final.Y);
}
