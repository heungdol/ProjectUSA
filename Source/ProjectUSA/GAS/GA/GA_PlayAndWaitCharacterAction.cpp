// Fill out your copyright notice in the Description page of Project Settings.


//#include "GAS/GA/GA_PlayAndWaitCharacterAction.h"
//
//#include "GAS/GA/GA_CharacterAction.h"
//#include "AbilitySystemComponent.h"
//
//#include "GameplayAbilitySpecHandle.h"
//
//#include "Data/USACharacterActionData.h"
//
//void UGA_PlayAndWaitCharacterAction::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
//}
//
//void UGA_PlayAndWaitCharacterAction::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
//}
//
//void UGA_PlayAndWaitCharacterAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//}
//
//void UGA_PlayAndWaitCharacterAction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
//{
//	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
//}
//
//void UGA_PlayAndWaitCharacterAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}
//
////
//
//void UGA_PlayAndWaitCharacterAction::GiveAbilitiesViaArray(class UUSACharacterActionData* ActionData)
//{
//	if (ActionData == nullptr)
//	{
//		return;
//	}
//
//	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
//
//	if (ASC == nullptr)
//	{
//		return;
//	}
//
//	for (const auto& Ability : ActionData->CharacterActionAbilities)
//	{
//		if (Ability == nullptr)
//		{
//			continue;
//		}
//
//		if (IsValid(Ability) == false)
//		{
//			continue;
//		}
//
//		FGameplayAbilitySpec GameplayAbilitySpec(Ability);
//		ASC->GiveAbility(GameplayAbilitySpec);
//	}
//}
//
//void UGA_PlayAndWaitCharacterAction::ClearAbilitiesViaArray(class UUSACharacterActionData* ActionData)
//{
//	if (ActionData == nullptr)
//	{
//		return;
//	}
//
//	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
//
//	if (ASC == nullptr)
//	{
//		return;
//	}
//
//	for (const auto& Ability : ActionData->CharacterActionAbilities)
//	{
//		if (Ability == nullptr)
//		{
//			continue;
//		}
//
//		if (IsValid(Ability) == false)
//		{
//			continue;
//		}
//
//		FGameplayAbilitySpec GameplayAbilitySpec(Ability);
//		ASC->ClearAbility(GameplayAbilitySpec.Handle);
//	}
//}
//
//void UGA_PlayAndWaitCharacterAction::SetCurrentCharacterActions(class UUSACharacterActionData* InCurrent)
//{
//	if (InCurrent == nullptr)
//	{
//		return;
//	}
//
//	CurrentCharacterActionData = InCurrent;
//}
//
//void UGA_PlayAndWaitCharacterAction::SetNextCharacterActions(class UUSACharacterActionData* InNext)
//{
//	if (InNext == nullptr)
//	{
//		return;
//	}
//
//	NextCharacterActionData = InNext;
//}
//
//void UGA_PlayAndWaitCharacterAction::SwapCurrentCharacterActionsFromNext()
//{
//	ClearAbilitiesViaArray(CurrentCharacterActionData);
//
//	CurrentCharacterActionData = NextCharacterActionData;
//
//	GiveAbilitiesViaArray(CurrentCharacterActionData);
//
//	NextCharacterActionData = nullptr;
//}
//
//void UGA_PlayAndWaitCharacterAction::ResetCurrentCharacterActionsToStart()
//{
//	ClearAbilitiesViaArray(CurrentCharacterActionData);
//
//	CurrentCharacterActionData = nullptr;
//
//	NextCharacterActionData = nullptr;
//}




