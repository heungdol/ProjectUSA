// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_CharacterStomp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterStomp : public UUSAGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_CharacterStomp();


	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)  override;


	///** Returns true if this ability can be activated right now. Has no side effects */
	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


public:
	UPROPERTY(EditAnywhere, Category = "Stomp Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PreGameplayEffects;

	UPROPERTY(EditAnywhere, Category = "Stomp Effect")
	TArray <TSubclassOf<class UGameplayEffect>> ActiveGameplayEffects;

	UPROPERTY(EditAnywhere, Category = "Stomp Effect")
	TArray <TSubclassOf<class UGameplayEffect>> EndGameplayEffects;

	UPROPERTY(EditAnywhere, Category = "Stomp Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PostGameplayEffects;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stomp)
	float StompMoveSpeed = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stomp)
	float StompPreDelay = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stomp)
	float StompPostDelay = 0.5f;

	//UPROPERTY()
	int8 bIsActivaed : 1;

protected:

	UFUNCTION()
	void ApplyPreEffects();

	UFUNCTION()
	void ApplyActiveEffects();

	UFUNCTION()
	void ApplyEndEffects();

	UFUNCTION()
	void ApplyPostEffects();

};
