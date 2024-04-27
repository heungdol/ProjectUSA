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
	virtual void PossessedBy(class AController* NewController) override;

protected:
	virtual void Look(const struct FInputActionValue& Value) override;

	virtual void SetupGAS() override;

};
