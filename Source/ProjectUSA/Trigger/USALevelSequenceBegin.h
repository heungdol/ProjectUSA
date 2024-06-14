// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/USALevelSequenceBase.h"
#include "USALevelSequenceBegin.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSALevelSequenceBegin : public AUSALevelSequenceBase
{
	GENERATED_BODY()
	
public:
	AUSALevelSequenceBegin();

protected:
	virtual void BeginPlay() override;

	void PlayBeginLevelSequence();
};
