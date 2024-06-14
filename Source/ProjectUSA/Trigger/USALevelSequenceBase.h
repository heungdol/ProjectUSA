// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USALevelSequenceBase.generated.h"

UCLASS()
class PROJECTUSA_API AUSALevelSequenceBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSALevelSequenceBase();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA LevelSequence Trigger Info")
	TObjectPtr <class UTextRenderComponent> LevelSequenceTriggerTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "USA LevelSequence Trigger Info")
	bool bIsActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA LevelSequence Trigger Info")
	bool bIsLocalEvent = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA LevelSequence Trigger Info")
	TObjectPtr<class ALevelSequenceActor> LevelSequenceActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
