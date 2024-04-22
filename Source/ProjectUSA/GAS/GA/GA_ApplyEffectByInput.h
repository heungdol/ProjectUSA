// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_ApplyEffectByInput.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FUSAGASDynamicGameplayTagDelegate, const FGameplayTagContainer&, int32);


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_ApplyEffectByInput : public UUSAGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_ApplyEffectByInput();

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;


public:

	UPROPERTY()
	bool bIsInputPressedOnce = false;
};
