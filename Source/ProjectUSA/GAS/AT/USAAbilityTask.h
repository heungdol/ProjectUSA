// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "USAAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUSAATSimpleDelegate);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	FUSAATSimpleDelegate OnAbilityTaskCancel;
	FUSAATSimpleDelegate OnAbilityTaskEnd;

	UFUNCTION()
	virtual void SimpleCancelAbilityTask();

	UFUNCTION()
	virtual void SimpleEndAbilityTask();


	void BroadcastSimpleDelegate(const FUSAATSimpleDelegate& InDelegate);

	virtual void Activate() override;

	bool bIsCancelable = true;
	
};
