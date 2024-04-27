// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/AsyncTaskGameplayAbilityEnded.h"

UAsyncTaskGameplayAbilityEnded* UAsyncTaskGameplayAbilityEnded::ListenForGameplayAbilityEnd(UAbilitySystemComponent* InASC, FGameplayTag GameplayTag_Task)
{
	if (!IsValid(InASC))
	{
		return nullptr;
	}

	if (GameplayTag_Task.IsValid() == false)
	{
		return nullptr;
	}

	UAsyncTaskGameplayAbilityEnded* AsyncTaskGameplayAbilityEnded = NewObject<UAsyncTaskGameplayAbilityEnded>();
	AsyncTaskGameplayAbilityEnded->ASC = InASC;
	AsyncTaskGameplayAbilityEnded->GameplayTag_Task = GameplayTag_Task;
	AsyncTaskGameplayAbilityEnded->DelegateHandle = InASC->RegisterGameplayTagEvent(GameplayTag_Task, EGameplayTagEventType::NewOrRemoved).AddUObject(AsyncTaskGameplayAbilityEnded, &UAsyncTaskGameplayAbilityEnded::OnCallback);

	return AsyncTaskGameplayAbilityEnded;
}

void UAsyncTaskGameplayAbilityEnded::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->UnregisterGameplayTagEvent(DelegateHandle, GameplayTag_Task, EGameplayTagEventType::NewOrRemoved);
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAsyncTaskGameplayAbilityEnded::OnCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount == 0) 
	{
		OnEnded.Broadcast();

		EndTask();
	}
}
