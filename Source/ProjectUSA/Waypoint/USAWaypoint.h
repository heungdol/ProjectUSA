// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USAWaypoint.generated.h"

UCLASS()
class PROJECTUSA_API AUSAWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAWaypoint();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Waypoint Info")
	TObjectPtr<class UBoxComponent> WaypointBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Waypoint Info")
	TObjectPtr<class UWidgetComponent> WaypointWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Waypoint Info")
	TObjectPtr<AUSAWaypoint> NextUSAWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Waypoint Info")
	bool bIsFirstWaypoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Waypoint Info")
	bool bIsLastWaypoint;


	UFUNCTION()
	void OnPlayerDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetActiveWaypoint(bool InActive);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_SetActiveWaypoint(bool InActive);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
