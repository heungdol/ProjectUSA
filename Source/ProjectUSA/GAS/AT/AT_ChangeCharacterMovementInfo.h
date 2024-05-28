// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"

#include "Struct/USAStructs.h"

#include "AT_ChangeCharacterMovementInfo.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_ChangeCharacterMovementInfo : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_ChangeCharacterMovementInfo* GetNewAbilityTask_ChangeCharacterMovementInfo(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, const FCharacterMovementWalkInfo& InInfo);

	virtual void Activate() override;
	//virtual void TickTask(float DeltaTime) override;

	FCharacterMovementWalkInfo ChangeCharacterMovementWalkInfo;
	FCharacterMovementWalkInfo DefaultCharacterMovementWalkInfo;

	TObjectPtr<class ACharacter> MyCharacter;


	void SetCharacterMovementInfo();
	void ResetCharacterMovementInfo();

	virtual void SimpleCancelAbilityTask() override;
	virtual void SimpleEndAbilityTask() override;
	
};