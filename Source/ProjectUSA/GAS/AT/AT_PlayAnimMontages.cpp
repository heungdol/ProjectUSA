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
		return;
	}

	if (PlayAnimMontageData == nullptr)
	{
		return;
	}


	bool bPlayedMontage = false;
	CurrentPlayAnimMontageIndex = -1;

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

	ACharacter* MyCharacter = nullptr;

	if (Ability != nullptr)
	{
		MyCharacter = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
	}

	if (MyCharacter != nullptr)
	{
		if (MyCharacter->PlayAnimMontage
		(PlayAnimMontageData->AnimMontage, 
			PlayAnimMontageData->AnimMontageRate, 
			PlayAnimMontageData->StartAnimMontageSectionDetail.SectionName))
		{
			bPlayedMontage = true;
			CurrentPlayAnimMontageIndex = 0;	
		}
	}

	if (!bPlayedMontage)
	{
		SimpleCancelAbilityTask();
		return;
	}

	if (PlayAnimMontageData->MiddleAnimMontageSectionDetails.Num() > 0)
	{
		float WaitTime = PlayAnimMontageData->MiddleAnimMontageSectionDetails[0].SectionPlayTime;

		GetWorld()->GetTimerManager().SetTimer(CallSectionTimerHandle, this, &UAT_PlayAnimMontages::OnSectionTimerHandleEnd, WaitTime, false);
	}

	//SetWaitingOnAvatar();
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
	
	if (MyCharacter != nullptr)
	{
		if (MyCharacter->GetCurrentMontage() == PlayAnimMontageData->AnimMontage)
		{
			if (PlayAnimMontageData->bHasEndSection)
			{
				MyCharacter->StopAnimMontage();
				MyCharacter->PlayAnimMontage
				(PlayAnimMontageData->AnimMontage, 
					PlayAnimMontageData->AnimMontageRate, 
					PlayAnimMontageData->EndAnimMontageSectionDetail.SectionName);
			}
			else
			{
				MyCharacter->StopAnimMontage();
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

void UAT_PlayAnimMontages::OnSectionTimerHandleEnd()
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
	//		ASC->CurrentMontageJumpToSection(PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionName);
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
			if (PlayAnimMontageData->bHasEndSection)
			{
				MyCharacter->StopAnimMontage();
				MyCharacter->PlayAnimMontage
				(PlayAnimMontageData->AnimMontage,
					PlayAnimMontageData->AnimMontageRate,
					PlayAnimMontageData->MiddleAnimMontageSectionDetails[CurrentPlayAnimMontageIndex].SectionName);
			}
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
	if (PlayAnimMontageData == nullptr)
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


//bool UAT_PlayAnimMontages::ServerRPC_Test_Validate()
//{
//	return true;
//}
//
//void UAT_PlayAnimMontages::ServerRPC_Test_Implementation()
//{
//	MulticastRPC_Test();
//}
//
//void UAT_PlayAnimMontages::MulticastRPC_Test_Implementation()
//{
//	USA_LOG_ABILITYTASK(LogTemp, Log, TEXT("From Anim Montage Task"));
//}
