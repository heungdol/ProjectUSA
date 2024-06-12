// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USATargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSATargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTUSA_API IUSATargetableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool GetIsTargetableCurrently() = 0;
	
	virtual FVector GetTargetablePivotlocation() = 0;

	virtual FVector GetTargetableToplocation() = 0;

	virtual float GetTargetableCapsuleRadius() = 0;


};
