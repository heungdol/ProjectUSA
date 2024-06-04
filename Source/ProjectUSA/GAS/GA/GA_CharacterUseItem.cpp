// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterUseItem.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Character/USACharacterBase.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"
#include "GAS/AT/AT_LaunchCharacterForPeriod.h"
#include "GAS/AT/AT_WaitDelay.h"
#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_WaitGameplayTag.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectUSA.h"

void UGA_CharacterUseItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 컴포넌트 설정
	ACharacter* MyCharacter = nullptr;
	AUSACharacterBase* MyUSACharacter = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
		MyUSACharacter = Cast <AUSACharacterBase>(MyCharacter);
	}

	if (IsValid(MyCharacter) == false)
	{
		SimpleCancelAbility();
		return;
	}

	if (IsValid(MyUSACharacter) == false)
	{
		SimpleCancelAbility();
		return;
	}

	if (MyUSACharacter->PreUseItem() == false)
	{
		SimpleCancelAbility();
		return;
	}

	// 종료 설정
	UAT_WaitDelay* AbilityTaskDelay = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, UseItemPeriod);
	AbilityTaskDelay->OnFinish.AddDynamic(this, &UGA_CharacterUseItem::SimpleEndAbility);
	AbilityTaskDelay->ReadyForActivation();

	// 무기 드롭 설정
	UAT_WaitDelay* AbilityTaskDelay_UseItem = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, UseItemTiming);
	AbilityTaskDelay_UseItem->OnFinish.AddDynamic(this, &UGA_CharacterUseItem::UseItemFromUSACharacter);
	AbilityTaskDelay_UseItem->ReadyForActivation();

	// 이동 설정
	UAT_ChangeCharacterMovementInfo* AbilityTask_ChangeMovementInfo = UAT_ChangeCharacterMovementInfo::GetNewAbilityTask_ChangeCharacterMovementInfo
	(this, MyCharacter, WalkMovementInfo);
	OnEndAbility.AddDynamic(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleEndAbilityTask);
	OnCancelAbility.AddDynamic(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleCancelAbilityTask);
	AbilityTask_ChangeMovementInfo->ReadyForActivation();

	// 애니메이션 설정
	//FPlayAnimMontageData AnimMontageData;
	//AnimMontageData.AnimMontage = ...;
	//AnimMontageData.AnimMontageRate = 1.0f;

	//UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, AnimMontageData);
	//OnEndAbility.AddDynamic(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	//OnCancelAbility.AddDynamic(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	//AbilityTaskMontage->ReadyForActivation();

	//MyUSACharacter->PreUseItem();

	K2_DoSomething_Activate();
}

void UGA_CharacterUseItem::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	K2_DoSomething_Cancel();
}

void UGA_CharacterUseItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	K2_DoSomething_End();
}


void UGA_CharacterUseItem::UseItemFromUSACharacter()
{
	// 컴포넌트 설정
	ACharacter* MyCharacter = nullptr;
	AUSACharacterBase* MyUSACharacter = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
		MyUSACharacter = Cast <AUSACharacterBase>(MyCharacter);
	}

	if (IsValid(MyUSACharacter) == false)
	{
		return;
	}

	MyUSACharacter->PostUseItem();
}
