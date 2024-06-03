// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"

#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_ChangeCharacterMovementInfo.h"

//#include "Interface/USATargetableInterface.h"

#include "Enum/USAEnums.h"

#include "GA_CharacterAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterAction : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:


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
	float Period = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action End")
	FGameplayTag EndGameplayTag;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action Interrupt")
	ECharacterActionEndType InterruptType = ECharacterActionEndType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action Interrupt")
	FGameplayTag InterruptGameplayTag;

	//


	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action Interrupt")
	//ECharacterActionEndType InterruptType = ECharacterActionEndType::WaitTagAdded;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action Interrupt")
	//float InterruptDelay = 0.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Action Interrupt")
	//FGameplayTag InterruptGameplayTag;


	//bool bIsInterruptDelayOver = false;
	//bool bIsInterruptCondtion = false;

	//FTimerHandle TimerHandleInterruptDelay;

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
	FVector CustomMoveAfterVelocity = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	float CustomMoveDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	UCurveFloat* CustomMoveCurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Move")
	class UCurveVector* CustomMoveCurveVector;

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
	struct FAttackTraceInfos AttackTraceData;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info: Attack")
	//bool bHasToAttachedAllWeaponToHolder = false;

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

	UFUNCTION()
	void AddArmorAttributeFromBase(float InAddArmor);

	UFUNCTION()
	void ResetArmorAttributeToBase();

	// ========================================================================

	//UFUNCTION()
	//void SetInterruptDelayOver();

	//UFUNCTION()
	//void SetInterruptCondition();
};
