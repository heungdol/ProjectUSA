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
	
//public:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//TArray <TSubclassOf<class UGameplayEffect>> InputPressedEffects;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//TArray <TSubclassOf<class UGameplayEffect>> InputReleasedEffects;


	// =====================================================================

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//FGameplayTagContainer InputPressedTags;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//FGameplayTagContainer InputHoldingTags;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//FGameplayTagContainer InputReleasedTags;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	//float RemoveTagDelay = 0.3f;

	//
	// =====================================================================

	UPROPERTY()
	bool bIsInputPressedOnce = false;

	//FTimerHandle InputPressedTimerHandle;
	//FTimerHandle InputReleasedTimerHandle;

	//FUSAGASDynamicGameplayTagDelegate OnAddGameplayTagCallback;
	//FUSAGASDynamicGameplayTagDelegate OnRemoveGameplayTagCallback;


public:
	UGA_ApplyEffectByInput();

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;


	//void AddAndRemoveReplicatedGameplayTagForPeriod(FTimerHandle& TimerHandle, const FGameplayTagContainer& InGameplayTag, float InPreiod);


	//TObjectPtr <class UAbilitySystemComponent> MyASC;

	//void SetupMyASC ();
};
