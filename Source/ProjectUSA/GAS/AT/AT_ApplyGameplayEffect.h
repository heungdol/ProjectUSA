// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_ApplyGameplayEffect.generated.h"

//USTRUCT(BlueprintType)
//struct FAT_ApplyGameplayEffectData
//{
//	GENERATED_BODY()
//	
//public:
//	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;
//
//};
//

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_ApplyGameplayEffect : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Wait specified time. This is functionally the same as a standard Delay node. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_ApplyGameplayEffect* GetNewAbilityTask(UGameplayAbility* OwningAbility, const TArray<TSubclassOf<class UGameplayEffect>>& InGameplayEffects);

	virtual void Activate() override;

//protected:
	//TArray<TSubclassOf<class FGameplayEffectSpecHandle>> GameplayEffectSpecs;
};
