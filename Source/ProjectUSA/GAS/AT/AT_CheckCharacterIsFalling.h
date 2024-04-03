// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_CheckCharacterIsFalling.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPositiveFalling);
DECLARE_MULTICAST_DELEGATE(FOnNegativeFalling);
DECLARE_MULTICAST_DELEGATE(FOnGrounded);
DECLARE_MULTICAST_DELEGATE(FOnFinished);

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_CheckCharacterIsFalling : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAT_CheckCharacterIsFalling* SpawnActor(UGameplayAbility* OwningAbility, /*class ACharacter* InCharacter,*/ class UCharacterMovementComponent* InCharacterMovement);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;
	

	FOnPositiveFalling OnPositiveFalling;
	FOnNegativeFalling OnNegativeFalling;
	FOnGrounded OnGrounded;
	FOnFinished OnFinished;


protected:
	//UPROPERTY ()
	//TObjectPtr <class ACharacter> MyCharacter;

	UPROPERTY()
	TObjectPtr <class UCharacterMovementComponent> MyCharacterMovementComponent;

	UPROPERTY ()
	bool bIsFalling = false;
};
