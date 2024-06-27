// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/USAGameInstance.h"

#include "AbilitySystemGlobals.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


//void UUSAGameInstance::Init()
//{
//    Super::Init();
//
//    //if (GetEngine())
//    //{
//    //    GetEngine()->OnTravelFailure().AddUObject(this, &UUSAGameInstance::HandleTravelFailure);
//    //}
//}

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

void UUSAGameInstance::DemoOneButtonStart()
{
    FString ServerIP;
    if (ReadTextFromFile(IPCONFIG_NAME, ServerIP) == false
        || GetIsValidIPAddress(ServerIP) == false)
    {
        DemoOpenLevelAsHost();
    }
    else
    {
        DemoJoinServer(ServerIP);
    }
}

void UUSAGameInstance::DemoJoinServer(const FString& InIpAddress)
{
    if (APlayerController* PlayerController = GetFirstLocalPlayerController())
    {
        FString URL = FString::Printf(TEXT("%s"), *InIpAddress);
        PlayerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
    }
}

void UUSAGameInstance::DemoOpenLevelAsHost()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName_MainGame), true, "listen");
}

bool UUSAGameInstance::GetIsDemo()
{
    FString DemoConfig;
    //FString FilePath = FPaths::ProjectDir() + "DemoConfig.txt";
    //bool bRead = FFileHelper::LoadFileToString(DemoConfig, *FilePath);
    if (ReadTextFromFile(DEMOCONFIG_NAME, DemoConfig) == false)
    {
        return false;
    }

    DemoConfig = DemoConfig.ToUpper();

    if (DemoConfig == "N"
        || DemoConfig == "NONE"
        || DemoConfig == "NULL"
        || DemoConfig == "NO"
        || DemoConfig == "NOT")
    {
        return false;
    }

    return true;
}

bool UUSAGameInstance::GetIsDemoHost()
{
    if (GetIsDemo() == false)
    {
        return false;
    }

    FString ServerIP;
    if (ReadTextFromFile(IPCONFIG_NAME, ServerIP) == false
        || GetIsValidIPAddress(ServerIP) == false)
    {
        return true;
    }

    return false;
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

FString UUSAGameInstance::GetPlayerNickByIndex(int32 InIndex)
{
    if (USANicknames.IsValidIndex(InIndex) == false)
    {
        return FString("Unknown");
    }

    return USANicknames[InIndex];
}

bool UUSAGameInstance::ReadTextFromFile(const FString& InFileName, FString& OutIPAddress)
{
    FString FilePath = FPaths::ProjectDir() + InFileName;
    return FFileHelper::LoadFileToString(OutIPAddress, *FilePath);
}

bool UUSAGameInstance::GetIsValidIPAddress(const FString& InIPAddress)
{
    TArray <FString> Parts;
    InIPAddress.ParseIntoArray(Parts, TEXT("."));

    if (Parts.Num() != 4)
    {
        return false;
    }

    for (const FString& Part : Parts)
    {
        if (Part.IsNumeric() == false)
        {
            return false;
        }

        int32 Number = FCString::Atoi(*Part);
        if (Number < 0 || 255 < Number)
        {
            return false;
        }
    }

    return true;
}

int32 UUSAGameInstance::GetScalabilitySettingNumber()
{
    FString SettingNumber;

    if (ReadTextFromFile(SCALABILITYCONFIG_NAME, SettingNumber) == false)
    {
        return 2;
    }

    if (SettingNumber.IsNumeric() == false)
    {
        return 2;
    }

    int32 ResultSettingNumber = FCString::Atoi(*SettingNumber);

    if (ResultSettingNumber < 0 || ResultSettingNumber >= 5)
    {
        return 2;
    }

    return ResultSettingNumber;
}
