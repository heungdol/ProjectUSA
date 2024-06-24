// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USALevelMusicInfo.generated.h"

UCLASS()
class PROJECTUSA_API AUSALevelMusicInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSALevelMusicInfo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Music Info");
	TObjectPtr<USoundBase> BackgroundSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Music Info");
	TObjectPtr<USoundBase> BackgroundMusic_Peaseful;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Music Info");
	TObjectPtr<USoundBase> BackgroundMusic_Boss;

};
