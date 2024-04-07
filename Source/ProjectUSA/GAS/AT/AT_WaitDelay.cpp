// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_WaitDelay.h"
#include "TimerManager.h"
#include "AbilitySystemGlobals.h"
#include "Engine/World.h"

UAT_WaitDelay::UAT_WaitDelay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Time = 0.f;
	TimeStarted = 0.f;
}

UAT_WaitDelay* UAT_WaitDelay::GetNewAbilityTask(UGameplayAbility* OwningAbility, float Time)
{
	//UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);

	UAT_WaitDelay* MyObj = NewAbilityTask<UAT_WaitDelay>(OwningAbility);
	MyObj->Time = Time;
	return MyObj;
}

void UAT_WaitDelay::Activate()
{
	if (Time < SMALL_NUMBER)
	{
		OnTimeFinish();
		return;
	}

	UWorld* World = GetWorld();
	TimeStarted = World->GetTimeSeconds();

	// Use a dummy timer handle as we don't need to store it for later but we don't need to look for something to clear
	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &UAT_WaitDelay::OnTimeFinish, Time, false);
}

void UAT_WaitDelay::OnTimeFinish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}

FString UAT_WaitDelay::GetDebugString() const
{
	if (UWorld* World = GetWorld())
	{
		const float TimeLeft = Time - World->TimeSince(TimeStarted);
		return FString::Printf(TEXT("WaitDelay. Time: %.2f. TimeLeft: %.2f"), Time, TimeLeft);
	}
	else
	{
		return FString::Printf(TEXT("WaitDelay. Time: %.2f. Time Started: %.2f"), Time, TimeStarted);
	}
}
