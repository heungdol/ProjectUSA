// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "USACharacterPivotComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTUSA_API UUSACharacterPivotComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSACharacterPivotComponent();

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "Pivot Component")
	FName PivotSocketName;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Pivot Component")
	TObjectPtr <class ACharacter> Character;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitPivotComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
