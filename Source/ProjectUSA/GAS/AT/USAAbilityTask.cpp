// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/USAAbilityTask.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"


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

void UUSAAbilityTask::BroadcastSimpleDelegate(const FUSAATSimpleDelegate& InDelegate)
{
	if (ShouldBroadcastAbilityTaskDelegates() == true)
	{
		InDelegate.Broadcast();
	}
}

void UUSAAbilityTask::Activate()
{
	Super::Activate();

	if (bIsCancelable)
	{
		if (AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UUSAAbilityTask::SimpleCancelAbilityTask);
		}
	}

	SetWaitingOnAvatar();
}
