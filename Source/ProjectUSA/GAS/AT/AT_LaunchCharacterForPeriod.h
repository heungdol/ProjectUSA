// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_LaunchCharacterForPeriod.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_LaunchCharacterForPeriod : public UUSAAbilityTask
{
	GENERATED_BODY()
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_LaunchCharacterForPeriod* GetNewAbilityTask(UGameplayAbility* OwningAbility, FVector InVelocity, bool InOverrideXY, bool InOverrideZ, float InPeriod);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void SimpleCancelAbilityTask() override;

	FOnSimpleDelegate OnFinished;


protected:
	UPROPERTY ()
	FVector LaunchVelocity;

	UPROPERTY()
	bool bXYOverride;

	UPROPERTY()
	bool bZOverride;

	UPROPERTY()
	float Period;


	UPROPERTY()
	float StartTime;

	UPROPERTY()
	float EndTime;
};
