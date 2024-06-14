// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/USALevelSequenceBase.h"
#include "USALevelSequenceEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSALevelSequenceEnd : public AUSALevelSequenceBase
{
	GENERATED_BODY()

public:
	AUSALevelSequenceEnd();


	UFUNCTION(BlueprintCallable)
	void PlayEndLevelSequence ();
};
