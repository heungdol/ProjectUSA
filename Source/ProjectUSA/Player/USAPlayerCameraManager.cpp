// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerCameraManager.h"

#include "Camera/USACameraShakeBase.h"

UCameraShakeBase* AUSAPlayerCameraManager::StartCameraShake(TSubclassOf<UCameraShakeBase> ShakeClass, float Scale, ECameraShakePlaySpace PlaySpace, FRotator UserPlaySpaceRot)
{
	// 레이어 작업
	UUSACameraShakeBase* USACameraShakeBase = Cast <UUSACameraShakeBase>(ShakeClass->GetDefaultObject());

	int8 CameraShakeBaseLayerIndex = -1;

	if (USACameraShakeBase != nullptr)
	{
		CameraShakeBaseLayerIndex = USACameraShakeBase->GetCameraShakeBaseLayerIndex();

		if (CameraShakeBaseLayerIndex >= 0)
		{
			if (CurrentCameraShakeBaseMap.Contains(CameraShakeBaseLayerIndex))
			{
				if (CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex] != nullptr)
				{
					StopCameraShake(CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex], true);
				}
			}
			else
			{
				CurrentCameraShakeBaseMap.Add({ CameraShakeBaseLayerIndex , nullptr });
			}
		}
	}

	// 실행
	UCameraShakeBase* InstCameraShakeBase = Super::StartCameraShake(ShakeClass, Scale, PlaySpace, UserPlaySpaceRot);

	if (CameraShakeBaseLayerIndex >= 0)
	{
		CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex] = InstCameraShakeBase;
	}

	return InstCameraShakeBase;
}
