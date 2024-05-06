// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/USAGameInstance.h"

#include "AbilitySystemGlobals.h"

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
