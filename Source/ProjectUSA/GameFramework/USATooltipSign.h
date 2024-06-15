// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USATooltipSign.generated.h"

UCLASS()
class PROJECTUSA_API AUSATooltipSign : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSATooltipSign();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Tooltip Sign Info")
	TObjectPtr <USceneComponent> TooltipSignHolderComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Tooltip Sign Info")
	TObjectPtr <class UWidgetComponent> TooltipSignWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Tooltip Sign Info")
	TObjectPtr <class UStaticMeshComponent> TooltipSignStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Tooltip Sign Info")
	FString TooltipText;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
