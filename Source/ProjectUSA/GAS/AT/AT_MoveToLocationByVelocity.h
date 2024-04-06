// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "AT_MoveToLocationByVelocity.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_MoveToLocationByVelocity : public UAbilityTask_MoveToLocation
{
	GENERATED_BODY()

public:
	/** Move to the specified location, using the vector curve (range 0 - 1) if specified, otherwise the float curve (range 0 - 1) or fallback to linear interpolation */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_MoveToLocationByVelocity* GetNewAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve);

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	virtual void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnCancelAbilityCallback ();

	UFUNCTION()
	void OnEndAbilityCallback();

protected:
	UPROPERTY(Replicated)
	FVector PrevLocation;

	//UPROPERTY()
	//FVector StartCharacterLocation;
};
