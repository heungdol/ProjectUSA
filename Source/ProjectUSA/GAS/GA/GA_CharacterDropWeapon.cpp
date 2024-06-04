// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterDropWeapon.h"

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


void UGA_CharacterDropWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	if (MyUSACharacter->GetIsThereAnyWeapon() == false)
	{
		SimpleCancelAbility();
		return;
	}

	// 종료 설정
	UAT_WaitDelay* AbilityTaskDelay = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, WeaponDropPeriod);
	AbilityTaskDelay->OnFinish.AddDynamic(this, &UGA_CharacterDropWeapon::SimpleEndAbility);
	AbilityTaskDelay->ReadyForActivation();

	// 무기 드롭 설정
	UAT_WaitDelay* AbilityTaskDelay_DropWeapon = UAT_WaitDelay::GetNewAbilityTask_WaitDelay(this, WeaponDropTiming);
	AbilityTaskDelay_DropWeapon->OnFinish.AddDynamic(this, &UGA_CharacterDropWeapon::DropWeaponFromUSACharacter);
	AbilityTaskDelay_DropWeapon->ReadyForActivation();

	// 이동 설정
	UAT_ChangeCharacterMovementInfo* AbilityTask_ChangeMovementInfo = UAT_ChangeCharacterMovementInfo::GetNewAbilityTask_ChangeCharacterMovementInfo
	(this, MyCharacter, WalkMovementInfo);
	OnEndAbility.AddDynamic(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleEndAbilityTask);
	OnCancelAbility.AddDynamic(AbilityTask_ChangeMovementInfo, &UAT_ChangeCharacterMovementInfo::SimpleCancelAbilityTask);
	AbilityTask_ChangeMovementInfo->ReadyForActivation();

	// 애니메이션 설정
	UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask_PlayAnimMontages(this, ActionAnimMontageData);
	OnEndAbility.AddDynamic(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	OnCancelAbility.AddDynamic(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
	AbilityTaskMontage->ReadyForActivation();

	K2_DoSomething_Activate();
}

void UGA_CharacterDropWeapon::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	K2_DoSomething_Cancel();
}

void UGA_CharacterDropWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	K2_DoSomething_End();
}

void UGA_CharacterDropWeapon::DropWeaponFromUSACharacter()
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

	MyUSACharacter->DropWeapons(true);
}

