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

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
};
