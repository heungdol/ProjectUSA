// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_TraceAttack.generated.h"

UENUM(BlueprintType)
enum class EAttackTraceShape : uint8
{
	None		UMETA(DisplayName = "None"),
	//Line		UMETA(DisplayName = "Line"),
	Sphere		UMETA(DisplayName = "Sphere"),
	//Box			UMETA(DisplayName = "Box"),
	//Capsule		UMETA(DisplayName = "Capsule")
};


USTRUCT(BlueprintType)
struct FAttackTraceInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector OffsetTraceLocation = FVector::ZeroVector;

	// 형태는?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	EAttackTraceShape TraceShape = EAttackTraceShape::Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector TraceSize = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackTime = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackDamage = -1.0f;
};


USTRUCT(BlueprintType)
struct FAttackTraceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Data")
	TArray <FAttackTraceInfo> AttackTraceInfos;

};


// ========================================================================================




/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_TraceAttack : public UUSAAbilityTask
{
	GENERATED_BODY()
		
public:
	UAT_TraceAttack();

	/** Wait specified time. This is functionally the same as a standard Delay node. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_TraceAttack* GetNewAbilityTask_TraceAttack(UGameplayAbility* OwningAbility, const FAttackTraceData& InAttackTraceData);

	virtual void Activate() override;

	void AttackTraceAndSetNextTimer();

public:
	const FAttackTraceData* AttackTraceData;

	FTimerHandle SpawnActorTimerHandle;

	float PrevAttackTraceTime = 0.0f;
	int CurrentAttackTraceIndex = 0;

	
};
