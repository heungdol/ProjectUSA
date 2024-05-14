// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_PlayCameraShake.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter* MyCharacter = Cast<ACharacter>(MeshComp->GetAttachParentActor());

	APlayerController* MyPlayerController = nullptr;

	if (MyCharacter != nullptr)
	{
		MyPlayerController = MyCharacter->GetController<APlayerController>();
	}

	if (MyPlayerController != nullptr
		&& MyPlayerController->PlayerCameraManager != nullptr)
	{
		MyPlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
}

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Notify(MeshComp, Animation);
}
