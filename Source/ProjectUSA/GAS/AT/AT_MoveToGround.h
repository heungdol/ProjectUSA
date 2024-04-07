// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_MoveToGround.generated.h"

// 땅에 닿을 시 호출할 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnBeginMovement);
DECLARE_MULTICAST_DELEGATE(FOnGroundReached);
//DECLARE_MULTICAST_DELEGATE(FOnEndTask);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_MoveToGround : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_MoveToGround* GetNewAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed/*, float InPreDelay, float InPostDelay*/);


	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnCancelAbilityCallback();

	UFUNCTION()
	void OnEndAbilityCallback();
	
	FOnBeginMovement OnBeginMovement;
	FOnGroundReached OnGroundReached;
	//FOnEndTask OnEndTask;

protected:
	UPROPERTY ()
	float MoveSpeed;

	//UPROPERTY()
	//float PreDelay;

	//UPROPERTY()
	//float PostDelay;

	int8 bIsFinished : 1;

	//float StepPreTime;
	//float StepActiveTime;
	//float StepEndTime;
	//float StepPostTime;
	//
	//int CurrentStep = -1;
};
