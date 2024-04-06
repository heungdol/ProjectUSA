// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_CheckCharacterIsFalling.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CheckCharacterIsFalling : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_CheckCharacterIsFalling();

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray <TSubclassOf<class UGameplayEffect>> PositiveFallingEffects;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray < TSubclassOf<class UGameplayEffect>> NegativeFallingEffects;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray < TSubclassOf<class UGameplayEffect>> GroundedEffects;

	UFUNCTION ()
	void OnPositiveFallingCallback();
	
	UFUNCTION()
	void OnNegativeFallingCallback();
	
	UFUNCTION()
	void OnGroundedCallback();

	UFUNCTION()
	void OnFinishedCallback();
};
