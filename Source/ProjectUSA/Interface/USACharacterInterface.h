// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USACharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UUSACharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTUSA_API IUSACharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetCharacterCurrentHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetCharacterMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetCharacterCurrentArmor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetCharacterCurrentHealthRatio();

};
