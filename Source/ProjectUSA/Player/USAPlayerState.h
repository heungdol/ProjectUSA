// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"

#include "USAPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUSAPlayerState();

public:
	UPROPERTY(/*Replicated, */EditAnywhere, Category = GAS)
	TObjectPtr <class UAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr <class UUSAAttributeSet> AttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex, VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	int32 PlayerIndex;

	void SetPlayerIndex(int32 InIndex);

	FORCEINLINE int32 GetPlayerIndex() { return PlayerIndex; }
	
	UFUNCTION()
	void OnRep_PlayerIndex();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Option")
	float LookSensitivityMouseRatio = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Option")
	float LookSensitivityGamepadRatio = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Option")
	bool bIsUsingGamepad = false;


public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	float GetLookSensitivityRatio();

	UFUNCTION(BlueprintCallable)
	void SetIsUsingGamepad(bool InUsing);

};