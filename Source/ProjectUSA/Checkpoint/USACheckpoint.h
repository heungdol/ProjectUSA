// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USACheckpoint.generated.h"

UCLASS()
class PROJECTUSA_API AUSACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Player Start Info")
	int32 CheckpointIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <USceneComponent> CheckpointHolderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class UBoxComponent> CheckpointBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Weapon Spawn Info")
	TObjectPtr <class UStaticMeshComponent> CheckpointStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Start Info")
	TObjectPtr <class UTextRenderComponent> CheckpointTextComponent;

	UFUNCTION()
	void OnDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE int32 GetCheckpointIndex() { return CheckpointIndex; }

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCheckpointUpdated(bool bIsActive);
};
