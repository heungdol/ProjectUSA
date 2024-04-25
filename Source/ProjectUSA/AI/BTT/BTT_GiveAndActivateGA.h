// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GiveAndActivateGA.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UBTT_GiveAndActivateGA : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_GiveAndActivateGA();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayAbility> InGameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InLevel /* = 0*/;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InInputID /* = -1*/ ;

	//struct FGameplayAbilitySpecHandle GameplayAbilitySpecHandle;

	TObjectPtr<class UGameplayAbility> PrimaryGameplayAbility;


public:
	TObjectPtr <class UAbilitySystemComponent> ASC;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnAbilityCancelCallback();

	UFUNCTION()
	void OnAbilityEndCallback(class UGameplayAbility* InGA);

	UFUNCTION()
	void OnAbilityEndStateCallback(FName InName);

};
