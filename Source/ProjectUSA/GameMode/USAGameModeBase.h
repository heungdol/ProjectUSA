// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "USAGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetAllPlayerControllerInput (bool InActive);

	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceToAllPlayer(class ALevelSequenceActor* InLevelSequnce);

	UFUNCTION(BlueprintCallable)
	void SetBossUSACharacter (class AUSACharacterBase* InCharacter);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Character");
	TObjectPtr<class AUSACharacterBase> BossUSACharacter;

};
