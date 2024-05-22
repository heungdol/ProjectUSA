// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"

#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_ChangeCharacterMovementInfo.h"

//#include "Interface/USATargetableInterface.h"

#include "GA_CharacterAction.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action End")
	ECharacterActionEndType EndType = ECharacterActionEndType::WaitTime;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action End")
	FGameplayTag EndGameplayTag;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	bool bIsMoveToTargetAction = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	FVector MoveToTargetAfterVelocity = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	float MoveToTargetDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	float MoveToTargetRange = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	float MoveToTargetGap = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	UCurveFloat* MoveToTargetCurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move To Target")
	class UCurveVector* MoveToTargetCurveVector;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	FVector MoveOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	FVector MoveAfterVelocity = FVector::ZeroVector;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	FCharacterMovementWalkInfo WalkMovementInfo;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Animation")
	struct FPlayAnimMontageData ActionAnimMontageData;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Spawn")
	struct FSpawnActorData SpawnActorData ;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Attack")
	struct FAttackTraceData AttackTraceData;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Attribute")
	float ArmorAttributeAddNumber = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Action Info: Attribute")
	bool bIsAppliedArmorAttribute = false;
	
	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: USA Jelly Effect")
	//TMap<float, TSubclassOf<class UUSAJellyEffectData>*>  USAJellyEffectData;

	//

public:

	// 애님 블루프린트는 함수 호출로 대체
	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_Activate();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_Cancel();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_End();


public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	virtual void CalculateTargetVector() override;
	virtual void DoSomethingWithTargetVector() override;
	virtual bool GetIsAbleToActivateCondition() override;

protected:

	UPROPERTY()
	TObjectPtr<class IUSATargetableInterface> TargetableActorInterface;

	UFUNCTION()
	void AddArmorAttributeFromBase(float InAddArmor);

	UFUNCTION()
	void ResetArmorAttributeToBase();
};
