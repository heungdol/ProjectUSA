// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "AT_TraceAttack.generated.h"

// 우선 Sphere 탐지만 진행
//UENUM(BlueprintType)
//enum class EAttackTraceShape : uint8
//{
//	None		UMETA(DisplayName = "None"),
//	//Line		UMETA(DisplayName = "Line"),
//	Sphere		UMETA(DisplayName = "Sphere"),
//	//Box			UMETA(DisplayName = "Box"),
//	//Capsule		UMETA(DisplayName = "Capsule")
//};


// =====================================================================


USTRUCT(BlueprintType)
struct FAttackTraceInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector OffsetTraceLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector OffsetTraceEndLocation = FVector::ZeroVector;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	bool bIsDirectToTarget = false;


	// 형태는?
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	//EAttackTraceShape TraceShape = EAttackTraceShape::Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackTraceRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackTime = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackDamage = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	TSubclassOf<UDamageType> AttackDamageType;

	
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



///** Damage subclass that handles damage with a single impact location and source direction */
//USTRUCT()
//struct ENGINE_API FUSADamageEvent : public FDamageEvent
//{
//	GENERATED_BODY()
//
//public:
//	/** Actual damage done */
//	UPROPERTY()
//	float Damage;
//
//	/** Direction the shot came from. Should be normalized. */
//	UPROPERTY()
//	FVector_NetQuantizeNormal ShotDirection;
//
//	/** Describes the trace/location that caused this damage */
//	UPROPERTY()
//	FHitResult HitInfo;
//
//	FUSADamageEvent() : Damage(0.0f), ShotDirection(FVector::ZeroVector), HitInfo() {}
//	FUSADamageEvent(float InDamage, const FHitResult& InHitInfo, FVector const& InShotDirection, TSubclassOf<UDamageType> InDamageTypeClass)
//		: FDamageEvent(InDamageTypeClass), Damage(InDamage), ShotDirection(InShotDirection), HitInfo(InHitInfo)
//	{}
//
//	/** ID for this class. NOTE this must be unique for all damage events. */
//	static const int32 ClassID = 1;
//
//	virtual int32 GetTypeID() const override { return FPointDamageEvent::ClassID; };
//	virtual bool IsOfType(int32 InID) const override { return (FPointDamageEvent::ClassID == InID) || FDamageEvent::IsOfType(InID); };
//
//	/** Simple API for common cases where we are happy to assume a single hit is expected, even though damage event may have multiple hits. */
//	//virtual void GetBestHitInfo(AActor const* HitActor, AActor const* HitInstigator, FHitResult& OutHitInfo, FVector& OutImpulseDir) const override;
//};


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
