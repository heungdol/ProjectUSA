// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "USACameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSACameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:

	class AActor* MainActor;

public:

	virtual void BeginPlay() override;

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bFOVOffsetByCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bUsingXYVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bUsingZVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float PrevFOVOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float DesiredFOVOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float CurrentFOVOffset;

	//

	//UPROPERTY (EditDefaultsOnly, BlueprintReadWrite)
	//float FOVOffsetMin = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float FOVOffsetMax = 20;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float FOVOffsetInterpSpeed = 1;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//float FOVOffsetMin = -5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float CharacterMovespeedMax = 1000;
};
