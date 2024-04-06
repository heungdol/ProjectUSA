// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_CharacterMoveInfoForPeriod.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_CharacterMoveInfoForPeriod : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_CharacterMoveInfoForPeriod* GetNewAbilityTask(UGameplayAbility* OwningAbility, ACharacter* InCharacter, class UCharacterMovementComponent* InCharacterMovement);

};
