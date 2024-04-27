// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AbilitySystemComponent.h"

#include "AsyncTaskGameplayAbilityEnded.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncTaskGameplayAbilityEndedEv);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAsyncTaskGameplayAbilityEnded : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FAsyncTaskGameplayAbilityEndedEv OnEnded;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskGameplayAbilityEnded* ListenForGameplayAbilityEnd(UAbilitySystemComponent* ASC,/*, TSubclassOf<UGameplayAbility> GameplayAbilityClass*/
		FGameplayTag GameplayTag_Task);

	void EndTask();
	
	FDelegateHandle DelegateHandle;

protected:

	UAbilitySystemComponent* ASC;
	FGameplayTag GameplayTag_Task;

	UFUNCTION()
	virtual void OnCallback(const FGameplayTag CallbackTag, int32 NewCount);
	
};
