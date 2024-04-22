// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_CharacterJump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterJump : public UUSAGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_CharacterJump ();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	//UFUNCTION()
	//void OnCancelAbilityCallback();

	//UFUNCTION()
	//void OnEndAbilityCallback ();

public:
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "Character Jump")
	float JumpPowerRatio = 1.0f;

//protected:
//	UPROPERTY ()
//	float MaxJumpInputTime;
//
	//UPROPERTY()
	//bool bIsJumping;
};
