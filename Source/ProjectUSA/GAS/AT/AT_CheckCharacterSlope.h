// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_CheckCharacterSlope.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSlopeTrue)
DECLARE_MULTICAST_DELEGATE(FOnSlopeFalse)
DECLARE_MULTICAST_DELEGATE(FOnGroundOut)


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_CheckCharacterSlope : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_CheckCharacterSlope* GetNewAbilityTask(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, float InStartSlopeAngle);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	FOnSlopeTrue OnSlopeTrue;
	FOnSlopeFalse OnSlopeFalse;
	FOnGroundOut OnGroundOut;
	
public:
	UFUNCTION()
	void OnCancelTaskCallback();

	UFUNCTION()
	void OnEndTaskCallback();

public:
	TObjectPtr <class ACharacter> MyCharacter;

	float StartSlopeAngle;

public:
	bool bIsCharacterOnSlopeWithForwardDirection;

	FORCEINLINE bool GetIsCharacterOnSlopeWithForwardDirection() const { return bIsCharacterOnSlopeWithForwardDirection; }


};
