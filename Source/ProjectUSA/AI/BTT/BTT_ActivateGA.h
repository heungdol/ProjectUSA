// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "BTT_ActivateGA.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UBTT_ActivateGA : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayAbility> InGameplayAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InLevel /* = 0*/;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InInputID /* = -1*/;

	//struct FGameplayAbilitySpecHandle GameplayAbilitySpecHandle;

	TObjectPtr<class UGameplayAbility> PrimaryGameplayAbility;

	FGameplayAbilitySpec AbilitySpec;

	bool bIsSetDelegates;
	
	
public:
	TObjectPtr <class UAbilitySystemComponent> ASC;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void OnAbilityCancelCallback();

	UFUNCTION()
	void OnAbilityEndCallback(class UGameplayAbility* InGA);
	
};
