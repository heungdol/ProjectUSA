// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "USACameraShakeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSACameraShakeBase : public UCameraShakeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = CameraShake)
	int8 CameraShakeBaseLayerIndex = -1;

public:
	FORCEINLINE int8 GetCameraShakeBaseLayerIndex() { return CameraShakeBaseLayerIndex;}
};
