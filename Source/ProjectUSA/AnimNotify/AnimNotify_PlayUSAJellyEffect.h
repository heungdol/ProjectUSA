// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayUSAJellyEffect.generated.h"

/**
 * 
 */

UCLASS(const, hidecategories = Object, collapsecategories, Config = Game, meta = (DisplayName = "Play USA Jelly Effect"))
class PROJECTUSA_API UAnimNotify_PlayUSAJellyEffect : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	TObjectPtr<class UUSAJellyEffectData> USAJellyEffectData;

public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
