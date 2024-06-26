// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AT/AT_PlayAnimMontages.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"

#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Character.h"

#include "ProjectUSA.h"


UAT_PlayAnimMontages* UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(UGameplayAbility* OwningAbility, const FPlayAnimMontageData& AnimMontageData)
{
	UAT_PlayAnimMontages* MyObj = NewAbilityTask<UAT_PlayAnimMontages>(OwningAbility);

	MyObj->PlayAnimMontageData = &AnimMontageData;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_PlayAnimMontages::Activate()
{
	if (Ability == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	if (PlayAnimMontageData == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	if (PlayAnimMontageData->AnimMontage == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	//CurrentPlayAnimMontageIndex = -1;

	//if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	//{
	//	if (ASC->PlayMontage(Ability
	//		, Ability->GetCurrentActivationInfo()
	//		, PlayAnimMontageData->AnimMontage
	//		, PlayAnimMontageData->AnimMontageRate
	//		, PlayAnimMontageData->StartAnimMontageSectionDetail.SectionName/*, StartTimeSeconds*/) > 0.0f)
	//	{
	//		bPlayedMontage = true;
	//		CurrentPlayAnimMontageIndex = 0;	
	//	}
	//}
	bool bPlayedMontage = false;

	UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();
	ACharacter* MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());

	FName StartSectionName = PlayAnimMontageData->StartAnimMontageSectionName;

	if (ASC)
	{
		TArray<FGameplayTag> KeyArray;
		PlayAnimMontageData->AnimMontageSectionMapByGameplayTagAdded.GenerateKeyArray(KeyArray);
		for (const FGameplayTag TagAdded : KeyArray)
		{
			FDelegateHandle DelegateHandle = ASC->RegisterGameplayTagEvent(TagAdded).AddUObject(this, &UAT_PlayAnimMontages::OnAnimSectionGameplayTagAdded);
			DelegateHandles.Add({ TagAdded, DelegateHandle });

			if (ASC->HasMatchingGameplayTag(TagAdded))
			{
				StartSectionName = PlayAnimMontageData->AnimMontageSectionMapByGameplayTagAdded[TagAdded];
			}
		}

		PlayAnimMontageData->AnimMontageSectionMapByGameplayTagRemoved.GenerateKeyArray(KeyArray);
		for (const FGameplayTag TagRemoved : KeyArray)
		{
			FDelegateHandle DelegateHandle = ASC->RegisterGameplayTagEvent(TagRemoved).AddUObject(this, &UAT_PlayAnimMontages::OnAnimSectionGameplayTagRemoved);
			DelegateHandles.Add({ TagRemoved, DelegateHandle });
		}
	}

	if (MyCharacter != nullptr)
	{

		if (MyCharacter->PlayAnimMontage
		(PlayAnimMontageData->AnimMontage, 
			PlayAnimMontageData->AnimMontageRate, 
			StartSectionName))
		{
			bPlayedMontage = true;
			//CurrentPlayAnimMontageIndex = 0;	
		}
	}

	if (!bPlayedMontage)
	{
		SimpleCancelAbilityTask();
		return;
	}

	//if (PlayAnimMontageData->MiddleAnimMontageSectionDetails.Num() > 0)
	//{
	//	float WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[0].SectionPlayTime;

	//	GetWorld()->GetTimerManager().SetTimer(CallSectionTimerHandle, this, &UAT_PlayAnimMontages::OnSectionTimerHandleEnd, WaitTime, false);
	//}

	
	SetWaitingOnAvatar();
}

void UAT_PlayAnimMontages::SimpleEndAbilityTask()
{
	if (PlayAnimMontageData == nullptr)
	{
		return;
	}

	//UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	//if (ASC)
	//{
	//	if (ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
	//	{
	//		if (PlayAnimMontageData->bHasEndSection)
	//		{
	//			ASC->CurrentMontageJumpToSection(PlayAnimMontageData->EndAnimMontageSectionDetail.SectionName);
	//		}
	//		else
	//		{
	//			ASC->CurrentMontageStop();
	//		}
	//	}
	//}

	ACharacter* MyCharacter = nullptr;
	
	if (Ability != nullptr)
	{
		MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
	}
	
	if (MyCharacter != nullptr
		&& PlayAnimMontageData->AnimMontage != nullptr)
	{
		if (MyCharacter->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			if (PlayAnimMontageData->bIsStopWhenFinished == true)
			{
				MyCharacter->StopAnimMontage();
			}
			else
			{
				if (IsValid (PlayAnimMontageData->EndAnimMontage) == true)
				{
					//MyCharacter->StopAnimMontage();
					MyCharacter->PlayAnimMontage
					(PlayAnimMontageData->EndAnimMontage,
						PlayAnimMontageData->AnimMontageRate);
				}
				else
				{
					// ...
				}
			}
		}
	}

	Super::SimpleEndAbilityTask();
}

void UAT_PlayAnimMontages::SimpleCancelAbilityTask()
{
	StopPlayingMontage();
	
	Super::SimpleCancelAbilityTask();
}

void UAT_PlayAnimMontages::OnAnimSectionGameplayTagAdded(FGameplayTag InTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		ACharacter* MyCharacter = nullptr;

		if (Ability != nullptr)
		{
			MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
		}

		if (MyCharacter == nullptr)
		{
			return;
		}

		FName SectionName = NAME_None;

		if (PlayAnimMontageData->AnimMontageSectionMapByGameplayTagAdded.Contains(InTag))
		{
			SectionName = PlayAnimMontageData->AnimMontageSectionMapByGameplayTagAdded[InTag];
		}

		MyCharacter->StopAnimMontage();
		MyCharacter->PlayAnimMontage
		(PlayAnimMontageData->AnimMontage,
			PlayAnimMontageData->AnimMontageRate,
			SectionName);
	}
}

void UAT_PlayAnimMontages::OnAnimSectionGameplayTagRemoved(const FGameplayTag InTag, int32 NewCount)
{
	if (NewCount <= 0)
	{
		ACharacter* MyCharacter = nullptr;

		if (Ability != nullptr)
		{
			MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
		}

		if (MyCharacter == nullptr)
		{
			return;
		}

		FName SectionName = NAME_None;

		if (PlayAnimMontageData->AnimMontageSectionMapByGameplayTagRemoved.Contains(InTag))
		{
			SectionName = PlayAnimMontageData->AnimMontageSectionMapByGameplayTagRemoved[InTag];
		}

		MyCharacter->StopAnimMontage();
		MyCharacter->PlayAnimMontage
		(PlayAnimMontageData->AnimMontage,
			PlayAnimMontageData->AnimMontageRate,
			SectionName);
	}
}

bool UAT_PlayAnimMontages::StopPlayingMontage()
{
	if (PlayAnimMontageData == nullptr)
	{
		return false;
	}

	if (PlayAnimMontageData->AnimMontage == nullptr)
	{
		return false;
	}

	//// Check if the montage is still playing
	//// The ability would have been interrupted, in which case we should automatically stop the montage
	//UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	//if (ASC)
	//{
	//	if (ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
	//	{
	//		ASC->CurrentMontageStop();
	//		ASC->ClearAnimatingAbility(Ability);
	//		
	//		return true;
	//	}
	//}

	ACharacter* MyCharacter = nullptr;

	if (Ability != nullptr)
	{
		MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
	}

	if (MyCharacter != nullptr)
	{
		if (MyCharacter->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			MyCharacter->StopAnimMontage();
			
			return true;
		}
	}

	return false;
}

void UAT_PlayAnimMontages::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);

	UAbilitySystemComponent* ASC = nullptr;
	if (Ability != nullptr)
	{
		ASC = Ability->GetAbilitySystemComponentFromActorInfo();
	}

	if (ASC)
	{
		TArray<FGameplayTag> KeyArray;
		DelegateHandles.GenerateKeyArray(KeyArray);

		for (FGameplayTag Tag : KeyArray)
		{
			ASC->RegisterGameplayTagEvent(Tag).Remove(DelegateHandles[Tag]);
		}
	}
}


//void UAT_PlayAnimMontages::OnSectionTimerHandleEnd()
//{
//	if (PlayAnimMontageData == nullptr)
//	{
//		return;
//	}
//
//	//UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
//	//if (ASC)
//	//{
//	//	if (ASC->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
//	//	{
//	//		ASC->CurrentMontageJumpToSection(PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionName);
//	//	}
//	//}
//
//	ACharacter* MyCharacter = nullptr;
//
//	if (Ability != nullptr)
//	{
//		MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
//	}
//
//	if (MyCharacter != nullptr)
//	{
//		if (MyCharacter->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
//		{
//			if (PlayAnimMontageData->bHasEndSection)
//			{
//				MyCharacter->StopAnimMontage();
//				MyCharacter->PlayAnimMontage
//				(PlayAnimMontageData->AnimMontage,
//					PlayAnimMontageData->AnimMontageRate,
//					PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionName);
//			}
//		}
//	}
//
//	CurrentPlayAnimMontageIndex += 1;
//	if (PlayAnimMontageData->MiddleAnimMontageSectionDetails.Num() <= CurrentPlayAnimMontageIndex)
//	{
//		return;
//	}
//
//	float WaitTime = 0.0f;
//	if (CurrentPlayAnimMontageIndex == 0)
//	{
//		WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionPlayTime;
//	}
//	else
//	{
//		WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionPlayTime
//			- PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex-1].SectionPlayTime;
//	}
//
//	GetWorld()->GetTimerManager().ClearTimer(CallSectionTimerHandle);
//	GetWorld()->GetTimerManager().SetTimer(CallSectionTimerHandle, this, &UAT_PlayAnimMontages::OnSectionTimerHandleEnd, WaitTime, false);
//
//}
