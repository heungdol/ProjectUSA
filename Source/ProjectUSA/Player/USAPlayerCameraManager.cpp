// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerCameraManager.h"

#include "Camera/USACameraShakeBase.h"

UCameraShakeBase* AUSAPlayerCameraManager::StartCameraShake
(TSubclassOf<UCameraShakeBase> ShakeClass, 
	float Scale, 
	ECameraShakePlaySpace PlaySpace, 
	FRotator UserPlaySpaceRot)
{
	// Layer를 판단하기 위함
	UUSACameraShakeBase* USACameraShakeBase 
		= Cast <UUSACameraShakeBase>(ShakeClass->GetDefaultObject());

	int8 CameraShakeBaseLayerIndex = -1;

	if (USACameraShakeBase != nullptr)
	{
		// Layer를 가져옴
		CameraShakeBaseLayerIndex 
			= USACameraShakeBase->GetCameraShakeBaseLayerIndex();

		// 유효한 Layer라면
		if (CameraShakeBaseLayerIndex >= 0)
		{
			// 만약 Map에서 해당 Layer에 대응하는 Key가 존재할 때
			if (CurrentCameraShakeBaseMap.Contains(CameraShakeBaseLayerIndex))
			{
				// 요청한 CameraShake의 Layer에 대응되는 기존의 CameraShake 존재할 때
				if (CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex] != nullptr)
				{
					// 기존의 CameraShake를 중단함
					StopCameraShake(CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex], true);
				}
			}
			// 요청한 CameraShake의 Layer에 대응하는 Key가 없는 경우
			else
			{
				CurrentCameraShakeBaseMap.Add({ CameraShakeBaseLayerIndex , nullptr });
			}
		}
	}

	// CameraShake 실행
	UCameraShakeBase* InstCameraShakeBase 
		= Super::StartCameraShake(ShakeClass, Scale, PlaySpace, UserPlaySpaceRot);

	// 현재 실행 중인 CameraShake 갱신
	if (CameraShakeBaseLayerIndex >= 0)
	{
		CurrentCameraShakeBaseMap[CameraShakeBaseLayerIndex] = InstCameraShakeBase;
	}

	return InstCameraShakeBase;
}
