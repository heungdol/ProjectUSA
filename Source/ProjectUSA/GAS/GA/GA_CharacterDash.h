// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GAS/AT/AT_PlayAnimMontages.h"

#include "GA_CharacterDash.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnSimpleDeletage)

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_CharacterDash : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_CharacterDash();
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)  override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//virtual void CalculateTargetDataVector(FVector& InOut) override;
	//virtual void DoSomethingWithTargetDataVector(const FVector& InVector) override;


	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPC_SetDashForwardDirection (const FVector& InDirection);

	virtual void CalculateTargetVector() override;
	virtual void DoSomethingWithTargetVector() override;
	virtual bool GetIsAbleToActivateCondition() override;


protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	//float DashDistance = 500.0f;
	FVector DashOffsetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	//float DashDistance = 500.0f;
	FVector MoveAfterVelocity = FVector::ZeroVector;
	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	float DashDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = GAS)
	UCurveFloat* DashCurveFloat;

	UPROPERTY(EditDefaultsOnly, Category = GAS)
	class UCurveVector* DashCurveVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
	struct FPlayAnimMontageData DashAnimMontageData;

	//UPROPERTY()
	//FVector DashForwardDirection = FVector::ForwardVector;
};
