// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USAPickableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSAPickableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTUSA_API IUSAPickableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool PickUpByUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter) = 0;

	virtual void PlayPickUpAnimationMontageInUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter) = 0;

	//virtual void SetPickableActorOwner(class AUSACharacterBase* InCharacter) = 0;

	//virtual void OnRep_PickableActorOwner(class AUSACharacterBase* PrevCharacter) = 0;

};
