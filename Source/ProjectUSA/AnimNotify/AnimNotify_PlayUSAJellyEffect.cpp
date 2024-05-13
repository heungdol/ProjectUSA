// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_PlayUSAJellyEffect.h"

#include "Component/USAJellyEffectComponent.h"
#include "Data/USAJellyEffectData.h"

void UAnimNotify_PlayUSAJellyEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* MeshOwner = MeshComp->GetAttachParentActor();
	UUSAJellyEffectComponent* JellyEffectComponent = nullptr;

	if (MeshOwner != nullptr)
	{
		JellyEffectComponent = Cast<UUSAJellyEffectComponent>(MeshOwner->GetComponentByClass(UUSAJellyEffectComponent::StaticClass()));
	}

	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->PlayJellyEffect(USAJellyEffectData);
		//UE_LOG(LogTemp, Log, TEXT("Play Jelly Notify"));

	}
}

void UAnimNotify_PlayUSAJellyEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Super::Notify(MeshComp, Animation, EventReference);

	//UE_LOG(LogTemp, Log, TEXT("Try Jelly Notify"));

	Notify(MeshComp, Animation);
}
