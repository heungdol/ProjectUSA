// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "USAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void BeginPlayingState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Player Controller Info");
	TObjectPtr <class UUserWidget> PlayerHUDUserWidget;
};
