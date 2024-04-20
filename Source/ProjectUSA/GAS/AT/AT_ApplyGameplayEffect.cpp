// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_ApplyGameplayEffect.h"

#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"


UAT_ApplyGameplayEffect* UAT_ApplyGameplayEffect::GetNewAbilityTask(UGameplayAbility* OwningAbility, const TArray<TSubclassOf<UGameplayEffect>>& InGameplayEffects)
{
	UAT_ApplyGameplayEffect* MyObj = NewAbilityTask<UAT_ApplyGameplayEffect>(OwningAbility);

	//MyObj->GameplayEffectSpecs.Init(nullptr, InGameplayEffects.Num());

	//for (int i = 0; i < InGameplayEffects.Num(); i++)
	//{
	//	if (InGameplayEffects[i] == nullptr)
	//	{
	//		continue;
	//	}

	//	//MyObj->GameplayEffectSpecs[i] = MyObj->Ability->MakeOutgoingGameplayEffectSpec(InGameplayEffects);
	//}

	return MyObj;
}

void UAT_ApplyGameplayEffect::Activate()
{
	Super::Activate();

	if (AbilitySystemComponent == nullptr)
	{
		SimpleCancelAbilityTask();
		return;
	}

	//for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : GameplayEffectClasses)
	//{
	//	FGameplayEffectSpecHandle GameplayEffectSpecHandle = 

	//	AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(GameplayEffectSpecHandle);

	//}

	SimpleEndAbilityTask();
}
