// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_ApplyEffectByInput.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_ApplyEffectByInput : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray <TSubclassOf<class UGameplayEffect>> InputPressedEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray <TSubclassOf<class UGameplayEffect>> InputReleasedEffects;

	UPROPERTY()
	bool bIsInputPressedOnce = false;
	
public:
	UGA_ApplyEffectByInput();

	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

};
