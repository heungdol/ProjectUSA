// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_WaitGameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_WaitGameplayTag : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:
	UAT_WaitGameplayTag();

	virtual void Activate() override;

	UFUNCTION()
	virtual void GameplayTagCallback(const FGameplayTag InTag, int32 NewCount);

	FGameplayTag	Tag;

protected:

	UAbilitySystemComponent* GetTargetASC();

	virtual void OnDestroy(bool AbilityIsEnding) override;
	bool RegisteredCallback;
	bool OnlyTriggerOnce;

	FDelegateHandle DelegateHandle;
};

/// ====================================================================================================

UCLASS()
class PROJECTUSA_API UAT_WaitGameplayTagAdded : public UAT_WaitGameplayTag
{
	GENERATED_BODY()

public:
	UAT_WaitGameplayTagAdded();

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	Added;

	/**
	 * 	Wait until the specified gameplay tag is Added. By default this will look at the owner of this ability. OptionalExternalTarget can be set to make this look at another actor's tags for changes.
	 *  If the tag is already present when this task is started, it will immediately broadcast the Added event. It will keep listening as long as OnlyTriggerOnce = false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WaitGameplayTagAdded* GetNewAbilityTask_WaitGameplayTagAdded(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce = false);

	virtual void GameplayTagCallback(const FGameplayTag InTag, int32 NewCount) override;
};

/// ====================================================================================================

UCLASS()
class PROJECTUSA_API UAT_WaitGameplayTagRemoved : public UAT_WaitGameplayTag
{
	GENERATED_BODY()

public:
	UAT_WaitGameplayTagRemoved();

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	Removed;

	/**
	 * 	Wait until the specified gameplay tag is Added. By default this will look at the owner of this ability. OptionalExternalTarget can be set to make this look at another actor's tags for changes.
	 *  If the tag is already present when this task is started, it will immediately broadcast the Added event. It will keep listening as long as OnlyTriggerOnce = false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WaitGameplayTagRemoved* GetNewAbilityTask_WaitGameplayTagRemoved(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce = false);

	virtual void GameplayTagCallback(const FGameplayTag InTag, int32 NewCount) override;
};