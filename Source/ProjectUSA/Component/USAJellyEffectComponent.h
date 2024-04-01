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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SetMeshComponent(class UMeshComponent* InMeshComponent);

	FORCEINLINE class UMeshComponent* GetMeshComponent() const { return MeshComponent; }


	virtual void PlayJellyEffect(class UUSAJellyEffectData* InJellyEffectData);

	virtual void StopJellyEffect();

	void TickJellyEffect();

	void TickJellyEffectByGravity();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jelly Effect")
	TObjectPtr <class UMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jelly Effect")
	TObjectPtr <class UUSAJellyEffectData> CurrentJellyEffectData;

	//

	bool bIsPlayingJellyEffect;

	float PlayJellyEffectTime;
	float EndJellyEffectTime;

	FVector StartMeshLocation;
	FRotator StartMeshRotation;
	FVector StartMeshScale;

	FVector CurrentJellyEffectLocation;
	FRotator CurrentJellyEffectRotation;
	FVector CurrentJellyEffectScale;


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	bool bIsUsingJellyEffectByGravity = false;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	float MaxGravityForScale = 3000;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	//float GravityInterpSpeed = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	TObjectPtr <class UCurveVector> ScaleByGravityRatio;

};
