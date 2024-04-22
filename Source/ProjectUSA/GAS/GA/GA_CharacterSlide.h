// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GAS/AT/AT_PlayAnimMontages.h"

#include "GA_CharacterSlide.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterSlide : public UUSAGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void DoSlide();

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UFUNCTION()
	void OnPreEndAbilityCallback();


	UFUNCTION()
	void OnSlopeTrue();

	UFUNCTION()
	void OnSlopeFalse();


	UFUNCTION()
	void OnCeilingTrue ();

	UFUNCTION()
	void OnCeilingFalse();


	UFUNCTION()
	void OnInputPressed();

	UFUNCTION()
	void OnInputReleased();


	UFUNCTION()
	void OnGroundOut ();


	void CheckAndRenewEndTimerHandle();

	FTimerHandle EndTimerHandle;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SetDirectionAndDoSlide (const FVector& InDirection);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlideStartPower = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlidePeriod = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlidePostPeriod = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlideStartAngle = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlideDetectCeilingHeight = 120.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	float SlideDetectCeilingRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	FGameplayTag SlideInputPressedTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	FGameplayTag SlideInputReleasedTag;


	UPROPERTY(EditAnywhere, Category = Slide)
	TArray <TSubclassOf<class UGameplayEffect>> InCeilingEffects;
	
	UPROPERTY(EditAnywhere, Category = Slide)
	TArray <TSubclassOf<class UGameplayEffect>> OutCeilingEffects;

	UPROPERTY(EditAnywhere, Category = Slide)
	TArray <TSubclassOf<class UGameplayEffect>> InSlopeEffects;

	UPROPERTY(EditAnywhere, Category = Slide)
	TArray <TSubclassOf<class UGameplayEffect>> OutSlopeEffects;

	UPROPERTY(EditAnywhere, Category = Slide)
	TArray <TSubclassOf<class UGameplayEffect>> OutGroundEffects;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slide)
	struct FPlayAnimMontageData SlideAnimMontageData;


protected:

	UPROPERTY()
	FVector SlideForwardDirection = FVector::ForwardVector;

	bool bIsSlope = false;
	bool bIsCeiling = false;
	bool bIsReleased = false;
	bool bIsGrounded = true;

	bool bIsOnceAcitved = false;

};
