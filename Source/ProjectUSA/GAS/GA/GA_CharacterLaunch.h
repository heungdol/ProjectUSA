// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_CharacterLaunch.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterLaunch : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	//UFUNCTION()
	//void OnCancelAbilityCallback();

	//UFUNCTION()
	//void OnEndAbilityCallback();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Launch)
	FVector LaunchVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Launch)
	bool bXYOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Launch)
	bool bZOverride;
	
};
