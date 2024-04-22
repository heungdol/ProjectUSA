// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GAS/AT/AT_PlayAnimMontages.h"

#include "GA_CharacterDash.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnSimpleDeletage)

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterDash : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_CharacterDash();
	
	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)  override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void DoDash();

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SetDashForwardDirection (const FVector& InDirection);


protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	//float DashDistance = 500.0f;
	FVector DashOffsetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	float DashDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = GAS)
	UCurveFloat* DashCurveFloat;

	UPROPERTY(EditDefaultsOnly, Category = GAS)
	class UCurveVector* DashCurveVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
	struct FPlayAnimMontageData DashAnimMontageData;

	UPROPERTY()
	FVector DashForwardDirection = FVector::ForwardVector;
};
