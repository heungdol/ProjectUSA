// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_LaunchCharacterForPeriod.generated.h"

DECLARE_MULTICAST_DELEGATE (FOnFinished)

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_LaunchCharacterForPeriod : public UAbilityTask
{
	GENERATED_BODY()
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_LaunchCharacterForPeriod* GetNewAbilityTask(UGameplayAbility* OwningAbility, FVector InVelocity, bool InOverrideXY, bool InOverrideZ, float InPeriod);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnCancelCallback();


	FOnFinished OnFinished;

protected:
	UPROPERTY ()
	FVector LaunchVelocity;

	UPROPERTY()
	bool bXYOverride;

	UPROPERTY()
	bool bZOverride;

	UPROPERTY()
	float Period;



	//UPROPERTY()
	//bool bIsSettingVelocity = false;

	UPROPERTY()
	float StartTime;

	UPROPERTY()
	float EndTime;




};
