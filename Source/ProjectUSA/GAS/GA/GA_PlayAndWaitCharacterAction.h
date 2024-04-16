// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "GAS/GA/USAGameplayAbility.h"
//#include "GA_PlayAndWaitCharacterAction.generated.h"
//
//
///**
// * 
// */
//UCLASS()
//class PROJECTUSA_API UGA_PlayAndWaitCharacterAction : public UUSAGameplayAbility
//{
//	GENERATED_BODY()
//	
//public:
//	
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Character Action Info")
//	class UUSACharacterActionData* StartCharacterActionData;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Character Action Info")
//	class UUSACharacterActionData* CurrentCharacterActionData;
//
//	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Character Action Info")
//	class UUSACharacterActionData* NextCharacterActionData;
//
//public:
//	/** Input binding stub. */
//	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
//
//	/** Input binding stub. */
//	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)  override;
//
//	/** Actually activate ability, do not call this directly */
//	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
//
//	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
//	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
//
//	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
//	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
//
//
//	UFUNCTION()
//	void GiveAbilitiesViaArray (class UUSACharacterActionData* ActionData);
//
//	UFUNCTION()
//	void ClearAbilitiesViaArray (class UUSACharacterActionData* ActionData);
//	
//	
//	UFUNCTION()
//	void SetCurrentCharacterActions(class UUSACharacterActionData* InCurrent);
//
//	UFUNCTION()
//	void SetNextCharacterActions(class UUSACharacterActionData* InNext);
//
//
//	UFUNCTION()
//	void SwapCurrentCharacterActionsFromNext();
//
//	UFUNCTION()
//	void ResetCurrentCharacterActionsToStart();
//
//};
