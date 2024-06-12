// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USAJellyEffectData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAJellyEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UUSAJellyEffectData();

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	//float JellyEffectActiveTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	float JellyEffectTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	bool bIsKeepLastEffect = false;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveLocationVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveRotationVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveScaleVector;


public:
	//FORCEINLINE float GetJellyEffectActiveTime() { return JellyEffectActiveTime; }
	FORCEINLINE float GetJellyEffectTime() { return JellyEffectTime; }

	FORCEINLINE bool GetJellyKeepLastEffect() { return bIsKeepLastEffect; }

	UFUNCTION()
	FVector GetLocationVectorByRatio(float InRatio);

	UFUNCTION()
	FVector GetRotationVectorByRatio(float InRatio);

	UFUNCTION()
	FVector GetScaleVectorByRatio(float InRatio);
	
};


//USTRUCT(BlueprintType)
//struct FUSAJellyEffectData
//{
//GENERATED_BODY()
//
//	public:
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Data")
//	TArray <UUSAJellyEffectData> USAJellyEffectDataList;
//
//};