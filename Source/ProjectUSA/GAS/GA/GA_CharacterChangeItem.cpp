// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterChangeItem.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Character/USACharacterBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectUSA.h"


void UGA_CharacterChangeItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	K2_DoSomething_Activate();

	ChangeItemFromUSACharacter();

	SimpleEndAbility();
}

void UGA_CharacterChangeItem::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	K2_DoSomething_Cancel();
}

void UGA_CharacterChangeItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	K2_DoSomething_End();
}


void UGA_CharacterChangeItem::ChangeItemFromUSACharacter()
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

	MyUSACharacter->ChangeItem (bIsChangeToNext);
}
