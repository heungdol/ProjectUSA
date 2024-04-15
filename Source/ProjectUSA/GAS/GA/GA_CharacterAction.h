// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"

#include "GAS/AT/AT_PlayAnimMontages.h"

#include "GA_CharacterAction.generated.h"

// 캐릭터 액션에서 필요한 동작
// 이동 (없음 / 단순 위치 이동 / 걷기 / Launch) -> 하나만 선택
// 애니메이션 (몽타주 / 애님 블루프린트 / 둘 다) -> 모두 열어 놓기
// 생성 (유 / 무) -> 하나만 선택
// 방향 (없음 / 입력 방향 / 적)

UENUM(BlueprintType)
enum class ECharacterActionMoveType : uint8
{
	None UMETA(DisplayName = "None"),
	Move UMETA(DisplayName = "Move"),
	Walk UMETA(DisplayName = "Walk"),
	Launch UMETA(DisplayName = "Launch"),
};

UENUM(BlueprintType)
enum class ECharacterActionDirectionType : uint8
{
	None UMETA(DisplayName = "None"),
	Input UMETA(DisplayName = "Input"),
	Target UMETA(DisplayName = "Target"),
};

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterAction : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: General")
	float Period = 0.5f;


	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Direction")
	ECharacterActionDirectionType DirectionType = ECharacterActionDirectionType::None;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	ECharacterActionMoveType MoveType = ECharacterActionMoveType::Move;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	FVector MoveOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	float MoveDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	UCurveFloat* MoveCurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	class UCurveVector* MoveCurveVector;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	FVector MoveLaunchVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	float MoveLaunchPeriod = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	bool bMoveLaunchXYOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	bool bMoveLaunchZOverride;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Animation")
	struct FPlayAnimMontageData ActionAnimMontageData;

	//

	// 스폰은 추후 구현

public:

	// 애님 블루프린트는 함수 호출로 대체
	UFUNCTION(BlueprintImplementableEvent)
	void SetAnimBlueprintProperties();


public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


};