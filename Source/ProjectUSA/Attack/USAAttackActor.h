// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/USASpawnableInterface.h"

#include "USAAttackActor.generated.h"

UCLASS()
class PROJECTUSA_API AUSAAttackActor : public AActor, public IUSASpawnableInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TObjectPtr<class UProjectileMovementComponent> AttackProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TObjectPtr<class USphereComponent> AttackSphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TObjectPtr<class UStaticMeshComponent> AttackStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TSubclassOf<AUSAAttackActor> NextSpawnAttackActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	float AttackDurtaion = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	float AttackTraceDurtaion = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	float AttackStartTime = 0.0f;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	//float AttackMoveSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	TSubclassOf<UDamageType> AttackDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	float AttackDamage = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsAttackableBySelf = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsDetectingStaticMesh = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsUsingSingleTrace = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TObjectPtr<AActor> OriginActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TObjectPtr<class AController> OriginController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	TArray<TObjectPtr<AActor>> OutActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsInitialized = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsHittingActor = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	bool bIsHittingStaticMeshActor = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	FVector PrevLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Attack Actor Info")
	float TraceRadius = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	TObjectPtr <class UNiagaraSystem> AttackHitNiagaraSystemObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	bool bIsAttackHitNiagaraOffset = true;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	//float AttackHitNiagaraSystemObjectRandomRatioX = 0.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	//float AttackHitNiagaraSystemObjectRandomRatioY = 0.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Attack Actor Info")
	//float AttackHitNiagaraSystemObjectRandomRatioZ = 0.0f;

public:	
	// Sets default values for this actor's properties
	AUSAAttackActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapTargetActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TryToGiveDamageToActor(AActor* InActor, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	virtual void Destroyed() override;

	virtual void InitUSASpawnableActor(AActor* InOriginActor, AController* InController);

	virtual bool GetIsInitialized();
};
