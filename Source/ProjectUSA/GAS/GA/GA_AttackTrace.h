// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/USAGameplayAbility.h"
#include "GA_AttackTrace.generated.h"

UENUM(BlueprintType)
enum class EUSAAttackTraceShape : uint8
{
	None UMETA(DisplayName = "None"),
	Sphere UMETA(DisplayName = "Sphere"),
	Box UMETA(DisplayName = "Box"),
	Ray UMETA(DisplayName = "Ray"),
};


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UGA_AttackTrace : public UUSAGameplayAbility
{
	GENERATED_BODY()
	
public:
	
};
