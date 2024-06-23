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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class UBoxComponent> WeaponBoxComponent;

public:

	virtual void BeginPlay() override;

	virtual void SetWeaponPhysics(bool IsDropping, bool IsFirst = false) override;

	//UFUNCTION(NetMulticast, Unreliable)
	//virtual void MulticastRPC_ImpulseWeapon(const FVector& InLocation, const FRotator& InRotatoin, const FVector& InScale, const FVector& InImpulse);

protected:

	UFUNCTION()
	void OnHitForPlayingImpactSound(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	float PlayImpactSoundCutOff = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	float PlayImpactSoundMax = 500.0f;

};

