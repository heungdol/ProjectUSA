// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USADamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSADamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTUSA_API IUSADamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) = 0;

	//virtual USceneComponent* GetDamageMesh() = 0;

	virtual void ApplyDamageHitNiagaraEffect(class AController* EventInstigator, class AActor* DamageCauser, class UNiagaraSystem* SystemTemplate, float OffsetRandomRatioX = 0.0f, float OffsetRandomRatioY = 0.0f, float OffsetRandomRatioZ = 0.0f) = 0;
};
