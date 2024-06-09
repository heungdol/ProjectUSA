// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Enum/USAEnums.h"
#include "Struct/USAStructs.h"

#include "USAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAHUD> LocalUSAHUD;

	virtual void BeginPlay() override;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintCallable)
	void SimulateClickMouseLeftButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SimulateClickMouseLeftButtonReleased();

	UFUNCTION(BlueprintCallable)
	void SimulateMoveMousePosition(FVector InDelta, float InSpeed, float InDeltaTime);

};
