// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USAWeaponSpawner.generated.h"

UCLASS()
class PROJECTUSA_API AUSAWeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAWeaponSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA Weapon Spawn Info")
	TSubclassOf <class AUSAWeaponBase> WeaponSpawnClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class UArrowComponent> WeaponSpawnArrowComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class UStaticMeshComponent> WeaponSpawnStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class USkeletalMeshComponent> WeaponSpawnSkeletalMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class UTextRenderComponent> WeaponSpawnTextComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
