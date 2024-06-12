// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/USAItemBase.h"
#include "USAItemGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAItemGameplayEffect : public AUSAItemBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TSubclassOf<class UGameplayEffect> ItemGameplayEffect;
	
	virtual void ActiveUSAItem(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter) override;
};
