// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_MoveToGround.generated.h"

// 땅에 닿을 시 호출할 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnGrounded);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_MoveToGround : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	FOnGrounded OnGrounded;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_MoveToGround* GetNewAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed/*, float InputBouncePower*/);

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	virtual void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

	//virtual void EndTask() override;

protected:
	float MoveSpeed;
	//float BouncePower;

	int8 bIsFinished : 1;
	
};
