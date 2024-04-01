// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USACharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSACharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual float GetGravityZ() const override;

	UPROPERTY(EditDefaultsOnly, Category = Gravity)
	float UppingGravityScale = 8.0f;

	UPROPERTY(EditDefaultsOnly, Category = Gravity)
	float DowningGravityScale = 5.0f;
	
};
