// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "USAGameplayAbility.generated.h"

DECLARE_MULTICAST_DELEGATE(FUSAGASimpleDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUSAGASimpleDynamicDelegate);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> ActivateAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> CancelAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PostCancelAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> EndAbilityEffects;

	UPROPERTY(EditAnywhere, Category = "Custom Active Effect")
	TArray <TSubclassOf<class UGameplayEffect>> PostEndAbilityEffects;


public:
	UUSAGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	// Client -(TargetData)-> Server 위한 함수
	void ActivateAbilityWithTargetData(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	//virtual void CalculateTargetDataVector(FVector& InOut);
	//virtual void DoSomethingWithTargetDataVector(const FVector& InVector);

	void ActivateAbilityUsingTargetVector(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
	virtual void CalculateTargetVector();
	virtual void DoSomethingWithTargetVector();
	virtual bool GetIsAbleToActivateCondition();

	FORCEINLINE FVector GetTargetVector() { return TargetVector; }
	FORCEINLINE float GetTargetDistance() { return TargetDistance; }

	FUSAGASimpleDelegate OnActivateAbility;
	FUSAGASimpleDelegate OnCancelAbility;
	FUSAGASimpleDelegate OnEndAbility;

	UFUNCTION()
	void SimpleCancelAbility ();

	UFUNCTION()
	void SimpleEndAbility();

private:
	//void ActivateAbilityWithTargetData_Client(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	//void NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
	//void ActivateAbilityWithTargetData_ClientServer(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetTargetVectorAndDoSomething(const FVector& InVector, float InDistance = -1.0f);
	
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_SetTargetVectorAndDoSomething(const FVector& InVector);


protected:
	UPROPERTY()
	FVector TargetVector;

	UPROPERTY()
	float TargetDistance = -1.0f;

	//UFUNCTION(BlueprintCallable)
	void ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects, 
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		float GameplayEffectLevel = 1.0f, 
		int32 Stacks = 1);

	UFUNCTION(BlueprintCallable)
	void ApplyEffectsViaArray(const TArray<TSubclassOf<class UGameplayEffect>>& GameplayEffects);
};
