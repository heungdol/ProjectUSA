// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USAItemSpawner.generated.h"

UCLASS()
class PROJECTUSA_API AUSAItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAItemSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA Item Spawn Info")
	TSubclassOf <class AUSAItemBase> ItemSpawnClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Item Spawn Info")
	TObjectPtr <class UArrowComponent> ItemSpawnArrowComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Item Spawn Info")
	TObjectPtr <class UStaticMeshComponent> ItemSpawnStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Item Spawn Info")
	TObjectPtr <class UTextRenderComponent> ItemSpawnTextComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};
