// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/USAGameplayAbility.h"

#include "AbilitySystemComponent.h"
//#include "Abilities/Tasks/AbilityTask.h"
//#include "GameplayTask.h"

//#include "GameplayAbilities/Public/Abilities/GameplayAbilityTypes.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/PlayerController.h"
//#include "AIModule/Classes/AIController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

#include "ProjectUSA.h"

//void UUSAGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
//}

UUSAGameplayAbility::UUSAGameplayAbility()
{
	// GAS 문서에서도 아래의 옵션은 설정하지 말라고 명시되어 있지만...
	// GameplayAbility간 RPC 통신을 위해서는 아래의 옵션을 켜줘야 한다.

	// 2024.05.04
	// 어빌리티에서 RPC 통신은 가능하지만, 기존의 언리얼에서 사용하는 방식은 오류를 발생시킬 수 있다.
	// 그러므로 ASC에서 사전 정의된, CallServerSetReplicatedTargetData, ConsumeClientReplicatedTargetData등의 함수를 활용할 것
	// 그래도 상식적으로 RPC를 이용하기 위해선 Replicate Policty를 켜줘야 한다고 판단

	// ... 그렇지만, RPC를 이용하는 것 보단 반응이 떨어지기 때문에, 사용자 경험에 부정적이라고 판단
	// 그러므로 RPC를 이용하는 것으로 코드 작성 수행

	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

//

void UUSAGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Activate Try Ability"));

	//AActor* MyASCActor = nullptr;
	//if (GetAbilitySystemComponentFromActorInfo() != nullptr)
	//{
	//	MyASCActor = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
	//}

	//if (UKismetSystemLibrary::IsServer(GetWorld()) == false
	//	&& MyASCActor != nullptr	
	//	&& MyASCActor->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	//{
	//	USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Server Try Ability"));

	//	GetAbilitySystemComponentFromActorInfo()->ServerTryActivateAbility_Validate(Handle, true, FPredictionKey());
	//}

	ApplyEffectsViaArray(ActivateAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnActivateAbility.Broadcast();
}

//

void UUSAGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	ApplyEffectsViaArray(CancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	OnCancelAbility.Broadcast();

	//OnDynamicCancelAbility.Broadcast();
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ApplyEffectsViaArray(PostCancelAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Cancel Ability"));
}

void UUSAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ApplyEffectsViaArray(EndAbilityEffects, Handle, ActorInfo, ActivationInfo);
	
	OnEndAbility.Broadcast();

	//OnDynamicEndAbility.Broadcast();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ApplyEffectsViaArray(PostEndAbilityEffects, Handle, ActorInfo, ActivationInfo);

	//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("End Ability"));
}

// ==============================================================================================================

void UUSAGameplayAbility::ActivateAbilityUsingTargetVector(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (GetIsAbleToActivateCondition() == false)
	{
		SimpleCancelAbility();
		return;
	}

	if (GetWorld() == nullptr)
	{
		SimpleCancelAbility();
		return;
	}

	ACharacter* AvatarCharacter = Cast <ACharacter>(GetAvatarActorFromActorInfo());
	
	AController* Controller = nullptr;
	APlayerController* PlayerController = nullptr;

	if (AvatarCharacter != nullptr)
	{
		Controller = AvatarCharacter->GetController();
	}

	if (Controller != nullptr)
	{
		PlayerController = Cast <APlayerController>(Controller);
	}

	if (Controller == nullptr)
	{
		SimpleCancelAbility();
		return;
	}

	//

	CalculateTargetVector();

	if (PlayerController != nullptr)
	{
		if (PlayerController->IsLocalController())
		{
			if (UKismetSystemLibrary::IsServer(GetWorld()))
			{
				DoSomethingWithTargetVector();
			}
			else
			{
				ServerRPC_SetTargetVectorAndDoSomething(GetTargetVector_Move(), GetTargetVector_Attack(), GetTargetDistance());

				DoSomethingWithTargetVector();
			}
		}
	}
	else
	{
		DoSomethingWithTargetVector();
	}
}

void UUSAGameplayAbility::ServerRPC_SetTargetVectorAndDoSomething_Implementation
(const FVector& InVector, const FVector& InVector_Attack, float InDistance)
{
	//MulticastRPC_SetTargetVectorAndDoSomething(InVector);
	
	TargetVector_Move = InVector;
	TargetVector_Attack = InVector_Attack;
	TargetDistance = InDistance;

	DoSomethingWithTargetVector();
}

// ==============================================================================================================


void UUSAGameplayAbility::CalculateTargetVector()
{
	// ...
}

void UUSAGameplayAbility::DoSomethingWithTargetVector()
{
	// ...
}

bool UUSAGameplayAbility::GetIsAbleToActivateCondition()
{
	return true;
}

// ==============================================================================================================

//void UUSAGameplayAbility::ActivateAbilityWithTargetData_Client(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	CalculateTargetVector();
//
//	FGameplayAbilityTargetDataHandle TargetDataHandle;
//	FGameplayAbilityTargetData_LocationInfo* TargetData = new FGameplayAbilityTargetData_LocationInfo();
//
//	TargetData->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
//	TargetData->TargetLocation.LiteralTransform = FTransform(TargetVector);
//
//	TargetDataHandle.Add(TargetData);
//
//	// Notify self (local client) *AND* server that TargetData is ready to be processed
//	NotifyTargetDataReady(TargetDataHandle, FGameplayTag());  // send with a gameplay tag, or empty
//}
//
//void UUSAGameplayAbility::NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
//{
//	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
//
//	if (ASC == nullptr)
//	{
//		SimpleCancelAbility();
//		return;
//	}
//
//	if (ASC->FindAbilitySpecFromHandle(CurrentSpecHandle) == nullptr)
//	{
//		SimpleCancelAbility();
//		return;
//	}
//
//	// if commit fails, cancel ability
//	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == false)
//	{
//		SimpleCancelAbility();
//		return;
//	}
//
//	// true if we need to replicate this target data to the server
//	const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
//
//	// Start a scoped prediction window
//	FScopedPredictionWindow	ScopedPrediction(ASC);
//
//	// Lyra does this memcopy operation; const cast paranoia is real. We'll keep it.
//	// Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
//	const FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));
//
//	// if this isn't the local player on the server, then notify the server
//	if (bShouldNotifyServer)
//	{
//		ASC->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, ASC->ScopedPredictionKey);
//	}
//
//	// Execute the ability we've now successfully committed
//	ActivateAbilityWithTargetData_ClientServer(LocalTargetDataHandle, ApplicationTag);
//
//	// We've processed the data, clear it from the RPC buffer
//	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
//}
//
//void UUSAGameplayAbility::ActivateAbilityWithTargetData_ClientServer(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag)
//{
//	// retrieve data
//	const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(0);
//	if (TargetData == nullptr)
//	{
//		SimpleCancelAbility();
//		return;
//	}
//
//	// decode data
//	//const FVector ClientVector = TargetData->GetEndPoint();
//	TargetVector = TargetData->GetEndPoint();
//
//	//// Server: Validate data
//	//const bool bIsServer = CurrentActorInfo->IsNetAuthority();
//	//if (bIsServer)
//	//{
//	//	if (ClientVector.X < 0)  // if negative X is prohibited by server for some reason
//	//	{
//	//		SimpleCancelAbility();
//	//		return;
//	//	}
//	//}
//
//	DoSomethingWithTargetVector();
//}
//
//void UUSAGameplayAbility::ActivateAbilityWithTargetData(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	if (UKismetSystemLibrary::IsServer(GetWorld()))
//	{
//		if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_Authority
//			&& GetAvatarActorFromActorInfo()->GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
//		{
//
//		}
//	}
//	else
//	{
//		if (GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
//		{
//			ActivateAbilityWithTargetData_Client(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//		}
//	}
//}

//void UUSAGameplayAbility::CalculateTargetDataVector(FVector& InOut)
//{
//	// ...
//}
//
//void UUSAGameplayAbility::DoSomethingWithTargetDataVector(const FVector& InVector)
//{
//	// ...
//}


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
	ServerRPC_ApplyEffectsViaArray(GameplayEffects);
}

void UUSAGameplayAbility::ServerRPC_ApplyEffectsViaArray_Implementation(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects)
{
	MulticastRPC_ApplyEffectsViaArray(GameplayEffects);
}
void UUSAGameplayAbility::MulticastRPC_ApplyEffectsViaArray_Implementation(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects)
{
	ApplyEffectsViaArray(GameplayEffects, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}
