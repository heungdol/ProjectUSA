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

	virtual void OnRep_bIsASCInitialized(bool Prev) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Non Player Info")
	float LifeSpanAfterDead = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Non Player Info")
	TObjectPtr<class UBehaviorTree> StartBehaviorTree;

protected:
	virtual void SetupGAS() override;

	virtual void UpdateCurrentTargetableActor() override;

	virtual void OnGameplayTagCallback_Dead(const struct FGameplayTag CallbackTag, int32 NewCount) override;
};
