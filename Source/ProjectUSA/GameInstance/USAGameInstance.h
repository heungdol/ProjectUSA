// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "USAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void OnStart() override;
};
