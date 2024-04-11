// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CheckCharacterCeiling.h"
#include "GAS/AT/AT_CheckCharacterCeiling.h"

#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

UGA_CheckCharacterCeiling::UGA_CheckCharacterCeiling()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_CheckCharacterCeiling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (MyCharacter == nullptr)
	{
		SimpleCancelAbility();
	}

	UCapsuleComponent* MyCharacterCapsuleComponent = MyCharacter->GetCapsuleComponent();
	if (MyCharacterCapsuleComponent == nullptr)
	{
		SimpleCancelAbility();
	}

	UAT_CheckCharacterCeiling* AbilityTask = UAT_CheckCharacterCeiling::GetNewAbilityTask
	(this, MyCharacter, MyCharacterCapsuleComponent->GetScaledCapsuleHalfHeight()*2.0f
		, MyCharacterCapsuleComponent->GetScaledCapsuleRadius());

	AbilityTask->OnCeilingTrue.AddUObject(this, &UGA_CheckCharacterCeiling::ApplayInCeilingEffects);
	AbilityTask->OnCeilingFalse.AddUObject(this, &UGA_CheckCharacterCeiling::ApplayOutCeilingEffects);

	AbilityTask->ReadyForActivation();
}

void UGA_CheckCharacterCeiling::ApplayInCeilingEffects()
{
	ApplyEffectsViaArray(InCeilingEffects);
}

void UGA_CheckCharacterCeiling::ApplayOutCeilingEffects()
{
	ApplyEffectsViaArray(OutCeilingEffects);
}
