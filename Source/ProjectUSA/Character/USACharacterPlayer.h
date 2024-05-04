// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USACharacterBase.h"
#include "USACharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSACharacterPlayer : public AUSACharacterBase
{
	GENERATED_BODY()
	
public:
	AUSACharacterPlayer();
	
public:
	virtual void OnRep_PlayerState() override;

protected:
	virtual void Look(const struct FInputActionValue& Value) override;

	virtual void SetupGAS() override;

	virtual void OnRep_ASC() override;
};
