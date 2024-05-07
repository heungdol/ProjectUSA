// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_WaitGameplayTag.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"


UAT_WaitGameplayTag::UAT_WaitGameplayTag()
{
	RegisteredCallback = false;
	OnlyTriggerOnce = false;
}

void UAT_WaitGameplayTag::Activate()
{
	UAbilitySystemComponent* ASC = GetTargetASC();
	if (ASC)
	{
		DelegateHandle = ASC->RegisterGameplayTagEvent(Tag).AddUObject(this, &UAT_WaitGameplayTag::GameplayTagCallback);
		RegisteredCallback = true;
	}
}

void UAT_WaitGameplayTag::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	// ...
}

UAbilitySystemComponent* UAT_WaitGameplayTag::GetTargetASC()
{
	return AbilitySystemComponent.Get();
}

void UAT_WaitGameplayTag::OnDestroy(bool AbilityIsEnding)
{
	UAbilitySystemComponent* ASC = GetTargetASC();
	if (RegisteredCallback && ASC)
	{
		ASC->RegisterGameplayTagEvent(Tag).Remove(DelegateHandle);
	}

	Super::OnDestroy(AbilityIsEnding);
}

/// ====================================================================================================

UAT_WaitGameplayTagAdded::UAT_WaitGameplayTagAdded()
{
}


UAT_WaitGameplayTagAdded* UAT_WaitGameplayTagAdded::GetNewAbilityTask_WaitGameplayTagAdded(UGameplayAbility* OwningAbility, FGameplayTag InTag, bool OnlyTriggerOnce)
{
	UAT_WaitGameplayTagAdded* MyObj = NewAbilityTask<UAT_WaitGameplayTagAdded>(OwningAbility);
	MyObj->Tag = InTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;

	return MyObj;
}

void UAT_WaitGameplayTagAdded::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	if (NewCount == 1)
	{
		BroadcastSimpleDelegate(Added);

		if (OnlyTriggerOnce)
		{
			EndTask();
		}
	}
}

/// ====================================================================================================

UAT_WaitGameplayTagRemoved::UAT_WaitGameplayTagRemoved()
{
}


UAT_WaitGameplayTagRemoved* UAT_WaitGameplayTagRemoved::GetNewAbilityTask_WaitGameplayTagRemoved(UGameplayAbility* OwningAbility, FGameplayTag InTag, bool OnlyTriggerOnce)
{
	UAT_WaitGameplayTagRemoved* MyObj = NewAbilityTask<UAT_WaitGameplayTagRemoved>(OwningAbility);
	MyObj->Tag = InTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;

	return MyObj;
}

void UAT_WaitGameplayTagRemoved::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		BroadcastSimpleDelegate(Removed);

		if (OnlyTriggerOnce)
		{
			EndTask();
		}
	}
}

