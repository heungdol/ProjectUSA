// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USAUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSACharacterBase> LocalUSACharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerController> LocalUSAPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAHUD> LocalUSAHUD;
	
	virtual void NativeOnInitialized() override;

	const int32 ScreenBumpOffset = 10;

public:

	UFUNCTION(BlueprintCallable)
	void SimulateClickMouseLeftButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SimulateClickMouseLeftButtonReleased();

	UFUNCTION(BlueprintCallable)
	void SimulateMoveMousePosition(FVector InDelta, float InSpeed, float InDeltaTime);

	//UFUNCTION(BlueprintCallable)
	//void UpdateTransformScaleByPlayerCamera(const FVector& InLocation);

protected:

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
