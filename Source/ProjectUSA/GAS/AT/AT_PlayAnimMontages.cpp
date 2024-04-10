// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AT/AT_PlayAnimMontages.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"

#include "Components/SkeletalMeshComponent.h"


UAT_PlayAnimMontages* UAT_PlayAnimMontages::GetNewAbilityTask(UGameplayAbility* OwningAbility, const FPlayAnimMontageData& AnimMontageData)
{
	UAT_PlayAnimMontages* MyObj = NewAbilityTask<UAT_PlayAnimMontages>(OwningAbility);

	MyObj->PlayAnimMontageData = &AnimMontageData;

	return MyObj;
}

void UAT_PlayAnimMontages::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	if (PlayAnimMontageData == nullptr)
	{
		return;
	}

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_PlayAnimMontages::OnCancelTaskCallback);
	}

	bool bPlayedMontage = false;
	CurrentPlayAnimMontageIndex = -1;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (ASC->PlayMontage(Ability
			, Ability->GetCurrentActivationInfo()
			, PlayAnimMontageData->AnimMontage
			, PlayAnimMontageData->AnimMontageRate
			, PlayAnimMontageData->StartAnimMontageSectionDetail.SectionName/*, StartTimeSeconds*/) > 0.0f)
		{
			bPlayedMontage = true;
			CurrentPlayAnimMontageIndex = 0;

		}
	}

	if (!bPlayedMontage)
	{
		OnCancelTaskCallback();
		return;
	}

	if (PlayAnimMontageData->MiddleAnimMontageSectionDetails.Num() > 0)
	{
		float WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[0].SectionPlayTime;

		GetWorld()->GetTimerManager().SetTimer(CallSectionTimerHandle, this, &UAT_PlayAnimMontages::OnSectionTimerHandleEnd, WaitTime, false);
	}

	// TODO 다른 어빌리티 태스크토 아래의 호출을 추가할 것
	SetWaitingOnAvatar();
}

void UAT_PlayAnimMontages::ExternalCancel()
{
	OnCancelTaskCallback();

	Super::ExternalCancel();
}

void UAT_PlayAnimMontages::OnEndTaskCallback()
{
	if (PlayAnimMontageData == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC/* && Ability*/)
	{
		if (/*(ASC->GetAnimatingAbility() == Ability || ASC->GetAnimatingAbility() == nullptr)
			&& */ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			if (PlayAnimMontageData->bHasEndSection)
			{
				ASC->CurrentMontageJumpToSection(PlayAnimMontageData->EndAnimMontageSectionDetail.SectionName);
			}
			else
			{
				ASC->CurrentMontageStop();
			}
		}
	}

	EndTask();
}

void UAT_PlayAnimMontages::OnCancelTaskCallback()
{
	StopPlayingMontage();

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast();
	}
}

void UAT_PlayAnimMontages::OnSectionTimerHandleEnd()
{
	if (PlayAnimMontageData == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC /*&& Ability*/)
	{
		if (/*ASC->GetAnimatingAbility() == Ability
			&& */ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			ASC->CurrentMontageJumpToSection(PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionName);
		}
	}

	CurrentPlayAnimMontageIndex += 1;
	if (PlayAnimMontageData->MiddleAnimMontageSectionDetails.Num() <= CurrentPlayAnimMontageIndex)
	{
		return;
	}

	float WaitTime = 0.0f;
	if (CurrentPlayAnimMontageIndex == 0)
	{
		WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionPlayTime;
	}
	else
	{
		WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionPlayTime
			- PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex-1].SectionPlayTime;
	}

	GetWorld()->GetTimerManager().ClearTimer(CallSectionTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CallSectionTimerHandle, this, &UAT_PlayAnimMontages::OnSectionTimerHandleEnd, WaitTime, false);

}

bool UAT_PlayAnimMontages::StopPlayingMontage()
{
	//if (Ability == nullptr)
	//{
	//	return false;
	//}

	if (PlayAnimMontageData == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC /*&& Ability*/)
	{
		if (/*(ASC->GetAnimatingAbility() == Ability || ASC->GetAnimatingAbility() == nullptr)
			&& */ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}