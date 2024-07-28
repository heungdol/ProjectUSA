// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/USATargetableInterface.h"

#include "Camera/PlayerCameraManager.h"

#include "USATargetingCameraActor.generated.h"

UCLASS()
class PROJECTUSA_API AUSATargetingCameraActor : public AActor
{
	GENERATED_BODY()
	
public:

	FORCEINLINE const FViewTargetTransitionParams& GetBeginViewTargetTransitionParams() const
	{ return BeginViewTargetTransitionParams; }

	FORCEINLINE const FViewTargetTransitionParams& GetEndViewTargetTransitionParams() const 
	{ return EndViewTargetTransitionParams; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr<AActor> SourceActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr<AActor> TargetActor;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	FViewTargetTransitionParams BeginViewTargetTransitionParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	FViewTargetTransitionParams EndViewTargetTransitionParams;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	float LocationWeightRatio = 0.2f;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxFOVOffset = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MinFOVOffset = 60;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxDistanceForFOV = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MinDistanceForFOV = 300;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float OffsetRotationYaw = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float PivotRotationPitch = -10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxRotationPitch = 45;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float MinRotationPitch = -45;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting Camera")
	float OffsetResultLocationHeight = 50.0f;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	TObjectPtr <class UUSAJellyEffectData> FocusShowJellyEffectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	TObjectPtr <class UUSAJellyEffectData> FocusHideJellyEffectData;


	bool bIsOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class USceneComponent> CameraFocusStaticMeshHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class UStaticMeshComponent> CameraFocusStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class UUSAJellyEffectComponent> CameraFocusJelleyEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class USoundBase> CameraFocusSoundOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class USoundBase> CameraFocusSoundOff;

public:	
	// Sets default values for this actor's properties
	AUSATargetingCameraActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	//void SetOnOff(bool InBool);
	
	UFUNCTION(BlueprintCallable)
	void SetSourceActor(AActor* const InActor);
	
	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* const InActor);

protected:
	void CalculateTargetingCameraTransform();
};
