// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USACharacterBase.h"
#include "USACharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSACharacterNonPlayer : public AUSACharacterBase
{
	GENERATED_BODY()

public:
	AUSACharacterNonPlayer();

public:
	UPROPERTY()
	TObjectPtr <class UUSAAttributeSet> AttributeSet;

	//virtual void OnRep_Controller() override;

	//virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_bIsASCInitialized() override;

protected:
	virtual void SetupGAS() override;
};
