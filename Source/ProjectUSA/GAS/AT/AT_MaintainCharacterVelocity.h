// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_MaintainCharacterVelocity.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_MaintainCharacterVelocity : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_MaintainCharacterVelocity* GetNewAbilityTask
	(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InVelocitySpeed, bool InIsConsiderZVelocity);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	float VelocitySpeed;
	bool bIsConsideringZVelocity;
};
