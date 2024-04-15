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

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category="Character Movement (General Settings)")
	float FallHaltHeight = 70;
	
public:
	virtual void PossessedBy(class AController* NewController) override;

protected:
	virtual void SetupGAS() override;

};
