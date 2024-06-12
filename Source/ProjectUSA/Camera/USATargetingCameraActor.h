// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/USATargetableInterface.h"

#include "USATargetingCameraActor.generated.h"

UCLASS()
class PROJECTUSA_API AUSATargetingCameraActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr<AActor> SourceActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr<AActor> TargetActor;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	float LocationWeightRatio = 0.2f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float RotationDistanceOffset = 300.0f;

	//

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float SourceHeightOffset = 30.0f;

	//

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float TargetMaxHeightOffset = 100.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float TargetMinHeightOffset = 0.0f;


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float TargetMaxVelocityZ = 500.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	//float TargetMinVelocityZ = 0.0f;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxFOVOffset = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MinFOVOffset = 60;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxDistanceForFOV = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MinDistanceForFOV = 300;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float OffsetRotationYaw = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float PivotRotationPitch = -10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MaxRotationPitch = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float MinRotationPitch = -45;

	//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	//float MinUpdateRange = 200;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting Camera")
	float OffsetResultLocationHeight = 50.0f;


	//bool bIsOn = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting Camera")
	TObjectPtr <class UStaticMeshComponent> CameraFocusStaticMeshComponent;


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
	void SetSourceActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* InActor);

protected:
	void CalculateTargetingCameraTransform();
};
