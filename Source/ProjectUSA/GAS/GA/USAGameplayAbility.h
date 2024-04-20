// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "USAGameplayAbility.generated.h"

DECLARE_MULTICAST_DELEGATE(FUSAGASimpleDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUSAGASimpleDynamicDelegate);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> ActivateAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> CancelAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PostCancelAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> EndAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PostEndAbilityEffects;
	

public:
	/** Does the commit atomically (consume resources, do cooldowns, etc) */
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	FUSAGASimpleDelegate OnActivateAbility;
	FUSAGASimpleDelegate OnCancelAbility; //-> OnGameplayAbilityCancelled
	FUSAGASimpleDelegate OnEndAbility; //-> OnGameplayAbilityEnded
	//

	UFUNCTION()
	void SimpleCancelAbility ();

	UFUNCTION()
	void SimpleEndAbility();


protected:
	//UFUNCTION(BlueprintCallable)
	void ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects
		, const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, float GameplayEffectLevel = 1.0f
		, int32 Stacks = 1);


	UFUNCTION(BlueprintCallable)
	void ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects);
};
