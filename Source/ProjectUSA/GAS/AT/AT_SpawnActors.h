// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_SpawnActors.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_SpawnActors : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_SpawnActors* GetNewAbilityTask_SpawnActors(UGameplayAbility* OwningAbility, float Time);

	
};
