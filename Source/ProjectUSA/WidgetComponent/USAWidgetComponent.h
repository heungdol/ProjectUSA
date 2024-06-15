// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "USAWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerCameraManager> LocalUSAPlayerCameraManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerController> LocalUSAPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Player Controller")
	float CullDistanceFromUSAPlayerCamera = 500.0f;
	
public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
