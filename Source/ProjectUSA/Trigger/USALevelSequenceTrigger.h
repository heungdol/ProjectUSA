// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/USALevelSequenceBase.h"
#include "USALevelSequenceTrigger.generated.h"

UCLASS()
class PROJECTUSA_API AUSALevelSequenceTrigger : public AUSALevelSequenceBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSALevelSequenceTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA LevelSequence Trigger Info")
	TObjectPtr<class UBoxComponent> TriggerBoxComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
