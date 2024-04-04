// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterStomp.h"


#include "GAS/AT/AT_MoveToGround.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "AbilitySystemBlueprintLibrary.h"


UGA_CharacterStomp::UGA_CharacterStomp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsActivaed = false;
}

void UGA_CharacterStomp::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterStomp::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UGA_CharacterStomp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return ActivateResult;
}

void UGA_CharacterStomp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		UAT_MoveToGround* AbilityTask = UAT_MoveToGround::GetNewAbilityTask
		(this, TEXT("Stomp"), StompMoveSpeed);

		AbilityTask->OnGrounded.AddUObject(this, &UGA_CharacterStomp::EndAbilityCallback);
		AbilityTask->ReadyForActivation();

		//MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
		//MyCharacterMovementComponent->Velocity = FVector::ZeroVector;
		bIsActivaed = true;
	}
	else
	{
		// Can not activate this ability 
		EndAbilityCallback();
	}
}

void UGA_CharacterStomp::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterStomp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bIsActivaed == (int8)true)
	{
		bIsActivaed = false;

		ACharacter* MyCharacter = nullptr;
		UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

		if (ActorInfo != nullptr)
		{
			MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
		}

		if (MyCharacter != nullptr)
		{
			MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
		}

		if (MyCharacterMovementComponent != nullptr)
		{
			MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);

			// 땅에 닿을 시 튀어오르도록
			MyCharacter->LaunchCharacter(FVector(0, 0, StompBouncePower), false, true);
		}
	}	
}

void UGA_CharacterStomp::EndAbilityCallback()
{
	// 네트워크 관련 인자
	bool bReplicatedEndAbility = true;
	// 취소된 경우
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
