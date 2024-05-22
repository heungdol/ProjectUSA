// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/USAWeaponBase.h"
#include "USAWeaponStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAWeaponStaticMesh : public AUSAWeaponBase
{
	GENERATED_BODY()
public:
	
	AUSAWeaponStaticMesh();

	void OnConstruction(const FTransform& Transform);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class UStaticMeshComponent> WeaponMeshComponent;

public:

	virtual void BeginPlay() override;

	virtual void SetWeaponPhysics(bool IsDropping, bool IsFirst = false) override;

};
