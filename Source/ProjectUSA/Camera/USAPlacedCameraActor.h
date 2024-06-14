// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Camera/PlayerCameraManager.h"

#include "USAPlacedCameraActor.generated.h"

UCLASS()
class PROJECTUSA_API AUSAPlacedCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAPlacedCameraActor();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placed Camera")
	//TArray<class UBoxComponent*> InOverlapBoxComponents;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placed Camera")
	//TArray<class UBoxComponent*> OutOverlapBoxComponents;

	FORCEINLINE FViewTargetTransitionParams GetBeginViewTargetTransitionParams() { return BeginViewTargetTransitionParams;}

	FORCEINLINE FViewTargetTransitionParams GetEndViewTargetTransitionParams() { return EndViewTargetTransitionParams; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	TObjectPtr <class USceneComponent> TotalHolderComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	//TObjectPtr <class UBoxComponent> InOverlapBoxComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	//TObjectPtr <class UBoxComponent> OutOverlapBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	TObjectPtr <class USceneComponent> InBoxHolderComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	//TObjectPtr <class USceneComponent> OutBoxHolderComponent;

	int32 PlayerOverlapCount = 0;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	TObjectPtr <class UArrowComponent> DirectionArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	TObjectPtr <class UCameraComponent> FrontCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placed Camera")
	TObjectPtr <class UCameraComponent> BackCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placed Camera")
	bool bIsBasedCameraRotation = true;

	//TObjectPtr <AActor> TargetActor;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	FViewTargetTransitionParams BeginViewTargetTransitionParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Camera")
	FViewTargetTransitionParams EndViewTargetTransitionParams;

	//

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitPlacedCameraActor(const FVector& InActorForward, const FVector& InCameraForward);

	FRotator GetActiveCameraRotation();

};
