// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/USAGameInstance.h"

#include "AbilitySystemGlobals.h"

#include "GameFramework/PlayerController.h"

void UUSAGameInstance::OnStart()
{
	Super::OnStart();

    auto& AbilitySystem = UAbilitySystemGlobals::Get();
    if (!AbilitySystem.IsAbilitySystemGlobalsInitialized())
    {
        // 어빌리티 관련 캐시 에러를 방지하기 위해 호출해야 하는 함수
        AbilitySystem.InitGlobalData();
    }
}

//

void UUSAGameInstance::SetUSAInputModeUI(APlayerController* InPlayerController)
{
    if (IsValid(InPlayerController) == false)
    {
        return;
    }

    if (InPlayerController->IsLocalPlayerController() == false)
    {
        return;

    }
    
    InPlayerController->bShowMouseCursor = true;

    InPlayerController->SetInputMode(FInputModeUIOnly());
}

void UUSAGameInstance::SetUSAInputModeGame(APlayerController* InPlayerController)
{
    if (IsValid(InPlayerController) == false)
    {
        return;
    }

    if (InPlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    InPlayerController->bShowMouseCursor = false;

    InPlayerController->SetInputMode(FInputModeGameOnly());
}

//

void UUSAGameInstance::HostSession(APlayerController* InPlayerController, FName InLevelName, bool InLAN)
{
    if (IsValid(InPlayerController) == false)
    {
        return;
    }

    if (InPlayerController->IsLocalPlayerController() == false)
    {
        return;
    }
    
    K2_HostSession(InPlayerController, InLevelName, InLAN);
}

void UUSAGameInstance::JoinSessionByLAN(APlayerController* InPlayerController)
{
    if (IsValid(InPlayerController) == false)
    {
        return;
    }

    if (InPlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    K2_JoinSessionByLAN(InPlayerController);
}

void UUSAGameInstance::JoinSessionByIPAddress(APlayerController* InPlayerController, const FText& InIP)
{
    if (IsValid(InPlayerController) == false)
    {
        return;
    }

    if (InPlayerController->IsLocalPlayerController() == false)
    {
        return;
    }

    K2_JoinSessionByIPAddress(InPlayerController, InIP);
}

//bool UUSAGameInstance::FunctionTestReturnBoolean_Implementation(int32 InValue)
//{
//    return false;
//}