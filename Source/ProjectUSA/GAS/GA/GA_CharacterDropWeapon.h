// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"

#include "GAS/AT/AT_PlayAnimMontages.h"
#include "GAS/AT/AT_SpawnActors.h"
#include "GAS/AT/AT_TraceAttack.h"
#include "GAS/AT/AT_ChangeCharacterMovementInfo.h"

#include "GA_CharacterDropWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterDropWeapon : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info")
	float WeaponDropPeriod = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info")
	float WeaponDropTiming = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info")
	FCharacterMovementWalkInfo WalkMovementInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Action Info")
	struct FPlayAnimMontageData ActionAnimMontageData;


public:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_Activate();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_Cancel();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_DoSomething_End();


public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void DropWeaponFromUSACharacter();
};
