// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "USAPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TMap <int8, UCameraShakeBase*> CurrentCameraShakeBaseMap;

public:
	virtual UCameraShakeBase* StartCameraShake(TSubclassOf<UCameraShakeBase> ShakeClass, float Scale = 1.f, ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal, FRotator UserPlaySpaceRot = FRotator::ZeroRotator) override;

	
};
