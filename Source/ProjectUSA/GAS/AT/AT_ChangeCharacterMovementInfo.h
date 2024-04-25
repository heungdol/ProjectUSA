// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_ChangeCharacterMovementInfo.generated.h"


USTRUCT(BlueprintType)
struct FCharacterMovementWalkInfo
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float MaxWalkSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	FRotator RotationRate = FRotator(0, 500, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float MaxAcceleration = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float GroundFriction = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float BrakingDecelerationWalking = 2000;

public:

	void RenewCharacterMovementInfo(class UCharacterMovementComponent* InMovementComponent);
};


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