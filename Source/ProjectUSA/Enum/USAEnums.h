// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 캡슐 중점을 설정하는 데에 필요
UENUM(BlueprintType)
enum class EUSACharacterCapsulePivot : uint8
{
	Top UMETA(DisplayName = "Top"),
	Center UMETA(DisplayName = "Center"),
	Bottom UMETA(DisplayName = "Bottom"),
};



// 캐릭터 액션에서 필요한 동작
// 이동 (없음 / 단순 위치 이동 / 걷기 / Launch) -> 하나만 선택
// 애니메이션 (몽타주 / 애님 블루프린트 / 둘 다) -> 모두 열어 놓기
// 생성 (유 / 무) -> 하나만 선택
// 방향 (없음 / 입력 방향 / 적)

UENUM(BlueprintType)
enum class ECharacterActionEndType : uint8
{
	None UMETA(DisplayName = "None"),
	WaitTime UMETA(DisplayName = "WaitTime"),
	WaitTagAdded UMETA(DisplayName = "WaitTagAdded"),
	WaitTagRemoved UMETA(DisplayName = "WaitTagRemoved"),
};

UENUM(BlueprintType)
enum class ECharacterActionMoveType : uint8
{
	None UMETA(DisplayName = "None"),
	Move UMETA(DisplayName = "Move"),
	Walk UMETA(DisplayName = "Walk"),
	Launch UMETA(DisplayName = "Launch"),
	//MoveToTarget UMETA(DisplayName = "MoveToTarget"), -> 우선 순위가 높으므로 변수들로 따로 빼 놓음
};

UENUM(BlueprintType)
enum class ECharacterActionDirectionType : uint8
{
	None UMETA(DisplayName = "None"),
	Input UMETA(DisplayName = "Input"),
	Target UMETA(DisplayName = "Target"),
	//Damage UMETA(DisplayName = "Damage"), <- 우선 CharacterBase에서 처리
};
