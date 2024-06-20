// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USAJellyEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTUSA_API UUSAJellyEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSAJellyEffectComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jelly Effect")
	TObjectPtr <class USceneComponent> JellySceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jelly Effect")
	TObjectPtr <class UUSAJellyEffectData> CurrentJellyEffectData;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	bool bIsUsingJellyEffectByGravity = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	float MaxGravityForScale = 3000;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	//float GravityInterpSpeed = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr <class UCurveVector> ScaleByGravityRatio;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//

	UFUNCTION(BlueprintCallable)
	virtual void SetJellySceneComponent(class ACharacter* InCharacter, class USceneComponent* InComponent);

	FORCEINLINE class USceneComponent* GetJellySceneComponent() const { return JellySceneComponent; }

	//

	void SetMeshStartLocation(FVector InVector);

	//
	UFUNCTION(BlueprintCallable)
	virtual void PlayJellyEffect(class UUSAJellyEffectData* InJellyEffectData);

	UFUNCTION(BlueprintCallable)
	virtual void StopJellyEffect();

	void TickJellyEffect();
	void TickJellyEffectByGravity();
	//void TickJellyEffectByCapsuleOffset();

	void TickJellyEffectFinal();

protected:

	bool bIsPlayingJellyEffect;

	float PlayJellyEffectTime;
	float EndJellyEffectTime;

	FVector StartMeshLocation;
	FRotator StartMeshRotation;
	FVector StartMeshScale;

	FVector CurrentJellyEffectLocation;
	FRotator CurrentJellyEffectRotation;
	FVector CurrentJellyEffectScale;

	FVector CurrentJellyEffectCapsuleOffsetLocation;
	FVector CurrentJellyEffectGravityScale;

};
