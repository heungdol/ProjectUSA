// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_WaitDelay.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_WaitDelay : public UUSAAbilityTask
{
	GENERATED_BODY()

public:
	UAT_WaitDelay(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate OnFinish;

	virtual void Activate() override;

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	/** Wait specified time. This is functionally the same as a standard Delay node. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WaitDelay* GetNewAbilityTask(UGameplayAbility* OwningAbility, float Time);

protected:

	void OnTimeFinish();

	float Time;
	float TimeStarted;
};
