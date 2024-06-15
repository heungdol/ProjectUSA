// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USACharacterSpawner.generated.h"

UCLASS()
class PROJECTUSA_API AUSACharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSACharacterSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA Character Spawn Info")
	TArray<TSubclassOf<class AUSACharacterNonPlayer>> CharacterSpawnClasses;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Character Spawn Info")
	TObjectPtr<class UBoxComponent> CharacterSpawnBoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Character Spawn Info")
	TObjectPtr <class UTextRenderComponent> CharacterSpawnTextComponent;


	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Character Spawn Info")
	//TArray<TObjectPtr<class UArrowComponent>> CharacterSpawnArrowComponents;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "USA Character Spawn Info")
	TArray<TObjectPtr<class USkeletalMeshComponent>> CharacterSpawnSkeletalMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USA Character Spawn Info")
	TObjectPtr<class ALevelSequenceActor> LevelSequenceActor;


	UPROPERTY()
	int32 CharacterSpawnNum = 10;

	UPROPERTY()
	bool bIsActivated = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_SpawnCharacterEvent ();

	//UFUNCTION(BlueprintImplementableEvent)
	//void K2_SpawnCharacterEvent();
};
