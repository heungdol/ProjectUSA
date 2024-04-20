// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ApplyEffectByInput.h"

#include "GameplayEffect.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"


#include "ProjectUSA.h"


/*

원초적으로 이펙트를 쓴 이유 -> ASC에 태그를 먹이고 삭제하기 위함
- 무한정 태그를 먹일 수도 있고
- 일정 시간동안도 태그를 먹일 수 있다

블루프린트를 이용한 이펙트를 적용하는 방법도 있지만... 뭔가 마음에 들지 않는다

어빌리티로 구현한다면 인식이나 반응속도는 빨라지지만, ASC에 어빌리티를 사전 등록하는 등의 복잡한 과정이 필요함
*/


UGA_ApplyEffectByInput::UGA_ApplyEffectByInput()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ApplyEffectByInput::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//SetupMyASC();
}

void UGA_ApplyEffectByInput::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	//SetupMyASC();

	//if (MyASC == nullptr)
	//{
	//	return;
	//}

	if (bIsInputPressedOnce == false)
	{
		//ApplyEffectsViaArray(InputPressedEffects);

		//SetCurrentInfo(Handle, ActorInfo, ActivationInfo);

		//AddAndRemoveReplicatedGameplayTagForPeriod(InputPressedTimerHandle, InputPressedTags, RemoveTagDelay);
		//MyASC->AddLooseGameplayTag(InputHoldingTag);

		// Remove tags we added to owner
		//MyASC->AddLooseGameplayTags(InputHoldingTags);

		//if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		//{
		//	MyASC->AddReplicatedLooseGameplayTags(InputHoldingTags);
		//}

		bIsInputPressedOnce = true;
	}
}

void UGA_ApplyEffectByInput::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	//SetupMyASC();

	//if (MyASC == nullptr)
	//{
	//	return;
	//}

	if (bIsInputPressedOnce == true)
	{
		//ApplyEffectsViaArray(InputReleasedEffects);

		//SetCurrentInfo(Handle, ActorInfo, ActivationInfo);

		//AddAndRemoveReplicatedGameplayTagForPeriod(InputReleasedTimerHandle, InputReleasedTags, RemoveTagDelay);
		//MyASC->RemoveLooseGameplayTag(InputHoldingTag);

		//MyASC->RemoveLooseGameplayTags(InputHoldingTags);

		//if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		//{
		//	MyASC->RemoveReplicatedLooseGameplayTags(InputHoldingTags);
		//}

		bIsInputPressedOnce = false;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


//void UGA_ApplyEffectByInput::AddAndRemoveReplicatedGameplayTagForPeriod
//(FTimerHandle& TimerHandle, const FGameplayTagContainer& InGameplayTag, float InPreiod)
//{
//	if (MyASC == nullptr)
//	{
//		return;
//	}
//
//	SetCurrentInfo(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
//
//	USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("ASC is Here"));
//
//	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
//	{
//		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
//		TimerHandle.Invalidate();
//
//		MyASC->RemoveLooseGameplayTags(InGameplayTag);
//	}
//	
//	OnAddGameplayTagCallback.Broadcast(InGameplayTag, 1);
//
//	FTimerDelegate TimerCallback;
//	TimerCallback.BindLambda([&] 
//		{
//			SetCurrentInfo(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
//
//			OnRemoveGameplayTagCallback.Broadcast(InGameplayTag, 1);
//		});
//
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, InPreiod, false);
//}
//
//void UGA_ApplyEffectByInput::SetupMyASC()
//{
//	if (MyASC == nullptr)
//	{
//		MyASC = GetAbilitySystemComponentFromActorInfo();
//
//		if (MyASC != nullptr)
//		{
//			OnAddGameplayTagCallback.AddUObject(MyASC, &UAbilitySystemComponent::AddLooseGameplayTags);
//			OnRemoveGameplayTagCallback.AddUObject(MyASC, &UAbilitySystemComponent::RemoveLooseGameplayTags);
//
//			//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Applicate Delegates"));
//		}
//	}
//}
