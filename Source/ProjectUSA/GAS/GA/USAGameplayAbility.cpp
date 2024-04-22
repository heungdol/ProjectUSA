// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/USAGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTask.h"

#include "ProjectUSA.h"

//void UUSAGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
//}

UUSAGameplayAbility::UUSAGameplayAbility()
{
	// GAS 문서에서도 아래의 옵션은 설정하지 말라고 명시되어 있지만...
	// GameplayAbility간 RPC 통신을 위해서는 아래의 옵션을 켜줘야 한다.
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//OnActivateAbility.AddUObject(this, &UUSAGameplayAbility::SetAbilityTasks_ReadyForActivation);
}

//

void UUSAGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyEffectsViaArray(ActivateAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnActivateAbility.Broadcast();

	//if (HasAuthority(&ActivationInfo))
	//{
	//	ActivateAbility_Server(Handle, ActorInfo, ActivationInfo);
	//}
	//else
	//{
	//	ActivateAbility_Client(Handle, ActorInfo, ActivationInfo);
	//}

	//ActivateAbility_Multicast(Handle, ActorInfo, ActivationInfo);
}

void UUSAGameplayAbility::External_ActivateUSAGameplayAbility()
{

}

//void UUSAGameplayAbility::ActivateAbility_Server(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//
//}
//
//void UUSAGameplayAbility::ActivateAbility_Client(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//
//}
//
//void UUSAGameplayAbility::ActivateAbility_Multicast(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	ApplyEffectsViaArray(ActivateAbilityEffects, Handle, ActorInfo, ActivationInfo);
//
//	//OnActivateAbility.Broadcast();
//}

//

void UUSAGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	ApplyEffectsViaArray(CancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnCancelAbility.Broadcast();
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ApplyEffectsViaArray(PostCancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Cancel Ability"));
}

void UUSAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ApplyEffectsViaArray(EndAbilityEffects, Handle, ActorInfo, ActivationInfo);
	
	OnEndAbility.Broadcast();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ApplyEffectsViaArray(PostEndAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("End Ability"));
}

//

void UUSAGameplayAbility::SimpleCancelAbility()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UUSAGameplayAbility::SimpleEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//

void UUSAGameplayAbility::ApplyEffectsViaArray
(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects
	, const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, float GameplayEffectLevel, int32 Stacks)
{
	for (const auto& GameplayEffectClass : GameplayEffects)
	{
		if (GameplayEffectClass == nullptr)
		{
			continue;
		}

		const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, GameplayEffect, GameplayEffectLevel, Stacks);
	}
}

void UUSAGameplayAbility::ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects)
{
	ApplyEffectsViaArray(GameplayEffects, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

//

//void UUSAGameplayAbility::AddAbilityTaskToArray(UAbilityTask* InTask)
//{
//	if (InTask == nullptr)
//	{
//		return;
//	}
//
//	AbilityTasks.Add(InTask);
//}
//
//void UUSAGameplayAbility::SetAbilityTasks_ReadyForActivation()
//{
//	for (UAbilityTask* Task : AbilityTasks)
//	{
//		if (Task == nullptr)
//		{
//			continue;
//		}
//
//		Task->ReadyForActivation();
//	}
//}
