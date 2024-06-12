// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "USAPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	AUSAPlayerStart(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Start Info")
	TObjectPtr <class UTextRenderComponent> PlayerStartTextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USA Player Start Info")
	int32 PlayerStartIndex = -1;

public:

	FORCEINLINE int32 GetPlayerStartIndex() { return PlayerStartIndex; }

};
