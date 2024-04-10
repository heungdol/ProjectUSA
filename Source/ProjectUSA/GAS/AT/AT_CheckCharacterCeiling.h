// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_CheckCharacterCeiling.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCeilingTrue)
DECLARE_MULTICAST_DELEGATE(FOnCeilingFalse)

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_CheckCharacterCeiling : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_CheckCharacterCeiling* GetNewAbilityTask(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, float InCharacterHeight, float InCharacterRadius);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	FOnCeilingTrue OnCeilingTrue;
	FOnCeilingFalse OnCeilingFalse;

public:
	UFUNCTION()
	void OnCancelTaskCallback();

	UFUNCTION()
	void OnEndTaskCallback();

public:
	TObjectPtr <class ACharacter> MyCharacter;
	float DetectCharacterHeight;
	float DetectCharacterRadius;

public:
	bool bIsCharacterCeilingDetected;
	FORCEINLINE bool GetIsCharacterCeilingDetected() const { return bIsCharacterCeilingDetected; }
};
