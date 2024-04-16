// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/USAAbilityTask.h"

void UUSAAbilityTask::SimpleCancelAbilityTask()
{
	BroadcastSimpleDelegate(OnAbilityTaskCancel);

	ExternalCancel();
}

void UUSAAbilityTask::SimpleEndAbilityTask()
{
	BroadcastSimpleDelegate(OnAbilityTaskEnd);

	EndTask();
}

void UUSAAbilityTask::BroadcastSimpleDelegate(const FOnSimpleDelegate& InDelegate)
{
	if (ShouldBroadcastAbilityTaskDelegates() == true)
	{
		InDelegate.Broadcast();
	}
}
