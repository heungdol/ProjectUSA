// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_CharacterLaunch.h"

#include "GameFramework/Character.h"


void UGA_CharacterLaunch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;

	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		FVector FinalLaunchVector = FVector::ZeroVector;

		FinalLaunchVector = LaunchVector.X * MyCharacter->GetActorForwardVector()
			+ LaunchVector.Y * MyCharacter->GetActorRightVector()
			+ LaunchVector.Z * FVector::UpVector;

		MyCharacter->LaunchCharacter(FinalLaunchVector, bXYOverride, bZOverride);
	}

	SimpleEndAbility();
}



void UGA_CharacterLaunch::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterLaunch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



//void UGA_CharacterLaunch::OnCancelAbilityCallback()
//{
//	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
//}
//
//void UGA_CharacterLaunch::OnEndAbilityCallback()
//{
//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
//}
