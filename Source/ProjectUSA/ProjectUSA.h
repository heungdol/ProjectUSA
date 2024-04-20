// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

//#include "Engine.h"


// 실행되는 함수의 클라 및 서버 정보를 가져오는 매크로
// UENUM이 아니기 때문에 직접 조건문을 통해 정보를 얻어야 함
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

#define LOG_NETMODEINFO_GAMEPLAYABILITY ((GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))


//3_04
// 넷모드의 Local 및 Remote의 Role을 가져오는 매크로
#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetRemoteRole()))

#define LOG_LOCALROLEINFO_GAMEPLAYABILITY *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetAvatarActorFromActorInfo()->GetLocalRole()))
#define LOG_REMOTEROLEINFO_GAMEPLAYABILITY *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetAvatarActorFromActorInfo()->GetRemoteRole()))

// 호출되는 함수의 정보를 가져오는 매크로
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

#define USA_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] [%s/%s] %s %s"), \
LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


#define USA_LOG_GAMEPLAYABILITY(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] [%s/%s] %s %s"), \
LOG_NETMODEINFO_GAMEPLAYABILITY, LOG_LOCALROLEINFO_GAMEPLAYABILITY, LOG_REMOTEROLEINFO_GAMEPLAYABILITY, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
