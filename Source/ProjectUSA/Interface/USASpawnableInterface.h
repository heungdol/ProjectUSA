// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USASpawnableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSASpawnableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTUSA_API IUSASpawnableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void InitUSASpawnableActor(AActor* InActor, class AController* InController) = 0;

	virtual bool GetIsInitialized() = 0;
};