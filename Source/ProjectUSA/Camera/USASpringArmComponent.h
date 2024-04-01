// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "USASpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSASpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	/** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	FVector OffsetLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	bool bIsUsingCustomLag = false;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	//float CameraYInterpSpeed = 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	//float PrevCameraYLocation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	//float DesiredCameraYLocation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	//float CurrentCameraYLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	float CameraInterpSpeed = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	FVector PrevCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	FVector DesiredCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	FVector CurrentCameraLocation;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	float CameraZUpOffset = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpringArm)
	float CameraZDownOffset = -50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	float DesiredCameraZLocation;

	//
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	//bool bWasCharacterFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm)
	bool bIsCharacterFalling  = false;
};
